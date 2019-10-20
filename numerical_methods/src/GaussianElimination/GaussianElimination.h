#pragma once

#include <cstdlib>

//
// Forward declarations
//

struct ProblemState;

//
// TODO: pivot right now implemented as-is would swap rows
// within the matrix. Consider augmenting the ProblemState s.t.
// a pivot vector would provide the corrected row order within
// the matrix.
//

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
bool ForwardSubstitution(ProblemState* pState, size_t rowIndex);

//
// In-place, turns the Uy=c system into Ib = b, effectively
// solving the original Ax=b system. Returns false if A is
// determined to be singular.
//
bool BackSubstitution(ProblemState* pState, size_t rowIndex);
