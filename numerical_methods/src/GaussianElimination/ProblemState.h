#pragma once

#include <memory>

//
// Represents the A and b of Ax = b
//
struct ProblemState
{
    std::unique_ptr<double[]> spMatrix;
    std::unique_ptr<double[]> spB;
    size_t                    MatrixSize;
};

//
// Helper class to index into the matrix. Really need to just create
// a matrix class, but for now this will do.
//
class MatrixIndexer
{
public:
    MatrixIndexer(ProblemState const* pState)
        : m_matrixSize(pState->MatrixSize) {}

    size_t Index(size_t rowIndex, size_t columnIndex)
    {
        return rowIndex * m_matrixSize + columnIndex;
    }

private:
    size_t m_matrixSize;
};
