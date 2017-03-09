#pragma once

#include <memory>

//
// TODO: pivot right now implemented as-is would swap rows
// within the matrix. Consider augmenting the ProblemState s.t.
// a pivot vector would provide the corrected row order within
// the matrix.
//

//
// Represents the A and b of Ax = b
//
struct ProblemState
{
    std::unique_ptr<double[]> spMatrix;
    std::unique_ptr<double[]> spB;
    int                       MatrixSize;
};

//
// Performs a partial pivot and permute the rows of the
// matrix in pState such that the diagonal entry of rowIndex
// is the largest it can be, accounting for all following
// rows.
//
void MaybePivot(ProblemState* pState, size_t rowIndex);

//
// In-place, turns the Ax=b system into upper-triangular
// Uy=c via forward substitution. Returns false if A is
// determined to be singular.
//
bool ForwardSubstitution(ProblemState* pState);

//
// In-place, turns the Uy=c system into Ib = b, effectively
// solving the original Ax=b system. Returns false if A is
// determined to be singular.
//
bool ForwardSubstitution(ProblemState* pState);
