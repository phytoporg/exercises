#include "InitializeSystem.h"
#include "ProblemState.h" // TODO: Factor ProblemState out of this file.

#include <random>

//
// If we generate a square matrix s.t. $\left|a_{ii}\right| <= R_i$ where 
// $R_i = \sum_{j \neq i} \left| a_{ij} \right|$, that matrix is invertible. 
//
void InitializeSystem(ProblemState* pState)
{
    //
    // Start out by initializing the 
    //
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(
        -20000.0, 20000.0
    );

    MatrixIndexer mi(pState);

    //
    // Populate the rows
    //
    for (size_t i = 0; i < pState->MatrixSize; i++)
    {
        pState->spB[i] = distribution(generator);
        for (size_t j = 0; j < pState->MatrixSize; j++)
        {
            pState->spMatrix[mi.Index(j, i)] = distribution(generator);
        }
    }

    //
    // Ensure the diagonal is sufficiently large for each row.
    //
    for (size_t i = 0; i < pState->MatrixSize; i++)
    {
        double sum = 0;
        for (size_t j = 0; j < pState->MatrixSize; j++)
        {
            if (i != j)
            {
                sum += abs(pState->spMatrix[mi.Index(j, i)]);
            }
        }

        double& diag = pState->spMatrix[mi.Index(i, i)];
        if (abs(diag) < sum)
        {
            if (diag < 0)
            {
                diag = -(sum + 1.0);
            }
            else
            {
                diag = sum + 1.0;
            }
        }
    }

    //
    // TODO: Permute rows to make Gaussian elimination more interesting (exercise pivots).
    //
}
