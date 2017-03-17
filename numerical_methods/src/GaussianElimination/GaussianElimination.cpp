#include "GaussianElimination.h"
#include "ProblemState.h"

#include <limits>
#include <cassert>

void MaybePivot(ProblemState* pState, size_t rowIndex)
{
    if (rowIndex >= (pState->MatrixSize - 1))
    {
        //
        // Nothing to do.
        //
        return;
    }

    MatrixIndexer mi(pState); // maybe just get rid of this?

    //
    // Find the largest value that would go into the (rowIndex, rowIndex)
    // diagonal slot prior to forward substitution.
    //

    const size_t ColIndex = rowIndex;
    size_t currentIndex = mi.Index(rowIndex, ColIndex);

    double maxElement = abs(pState->spMatrix[currentIndex]);
    size_t maxRowIndex = rowIndex;
    for (size_t r = rowIndex + 1; r < pState->MatrixSize; r++)
    {
        const double& CurrentElement = abs(pState->spMatrix[currentIndex]);
        if (maxElement < CurrentElement)
        {
            maxElement = CurrentElement;
            maxRowIndex = r;
        }

        currentIndex += pState->MatrixSize;
    }

    if (maxRowIndex != rowIndex)
    {
        //
        // Swap rows if there's a new max.
        //
        double* pCurrentRow = &pState->spMatrix[mi.Index(rowIndex, 0)];
        double* pMaxRow     = &pState->spMatrix[mi.Index(maxRowIndex, 0)];

        static std::unique_ptr<double[]> spScratchRow(new double[pState->MatrixSize]);
        const size_t RowSize = sizeof(double) * pState->MatrixSize;

        double* pTempBuffer = spScratchRow.get();
        // temp = current
        memcpy(pTempBuffer, pCurrentRow, RowSize);
        // current = max
        memcpy(pCurrentRow, pMaxRow, RowSize);
        // max = temp
        memcpy(pMaxRow, pTempBuffer, RowSize);

        //
        // Also swap corresponding elements in b
        //
        std::swap(pState->spB[rowIndex], pState->spB[maxRowIndex]);
    }
}

bool ForwardSubstitution(ProblemState* pState, size_t rowIndex)
{
    MatrixIndexer mi(pState); // maybe just get rid of this?

    //
    // Normalize the current row wrt to the diagonal element.
    //
    const size_t DiagIndex = mi.Index(rowIndex, rowIndex);
    double* pCurrentRowDiag = &pState->spMatrix[DiagIndex];
    const double DiagElement = *pCurrentRowDiag;

    if (abs(DiagElement) <= std::numeric_limits<double>::epsilon())
    {
        //
        // Singular matrix. Bail.
        //
        return false;
    }

    for (size_t c = 0; c < pState->MatrixSize - rowIndex; c++)
    {
        pCurrentRowDiag[c] /= DiagElement;
    }
    pState->spB[rowIndex] /= DiagElement;

    //
    // Eliminate all forward elements in this column by subtracting from its row
    // a choice multiple of the current row.
    //
    for (size_t r = rowIndex + 1; r < pState->MatrixSize; r++)
    {
        double* pThisRowDiagCol = &pState->spMatrix[mi.Index(r, rowIndex)];
        const double EliminationValue = *pThisRowDiagCol;

        for (size_t c = 0; c < pState->MatrixSize - rowIndex; c++)
        {
            *pThisRowDiagCol -= EliminationValue * pCurrentRowDiag[c];
            ++pThisRowDiagCol;
        }

        pState->spB[r] -= EliminationValue * pState->spB[rowIndex];
    }

    return true;
}

bool BackSubstitution(ProblemState* pState, size_t rowIndex)
{
    //
    // For each row starting at the bottom, clear out the remaining
    // off-diagonal elements via elementary row operations.
    //
    // We're assuming here that all remaining diagonal entries are 
    // unitary.
    //
    MatrixIndexer mi(pState); // maybe just get rid of this?

    double *pColEntry = &pState->spMatrix[mi.Index(rowIndex, rowIndex)];
    const double DiagEntry = *pColEntry;
    assert(abs(DiagEntry - 1.0) <= std::numeric_limits<double>::epsilon());
    for (int r = static_cast<int>(rowIndex) - 1; r >= 0; --r)
    {
        pColEntry -= pState->MatrixSize;

        const double ToSubtract = DiagEntry * *pColEntry;
        pState->spB[r] -= ToSubtract;
        *pColEntry     -= ToSubtract;
    }

    return true;
}
