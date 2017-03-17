#pragma once

//
// Forward declarations
//
struct ProblemState;

//
// Randomly initializes a square matrix s.t. it is nonsingular using
// Gershgorgin's circle theorem. 
// 
// See https://matthewhr.wordpress.com/2013/09/01/how-to-construct-an-invertible-matrix-just-choose-large-diagonals/ 
// for details.
//
void InitializeSystem(ProblemState* pState);
