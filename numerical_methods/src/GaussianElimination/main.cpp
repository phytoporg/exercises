//
// Entry point for performing Gaussian Elimination to solve linear systems of
// the form Ax = b. 
//
// This program will generate a number of matrices of a user-specified size
// and a vector to solve for and print the solutions if any exist.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include <limits>

#include "GaussianElimination.h"
#include "InitializeSystem.h"
#include "ProblemState.h"

//
// Forward declarations
//

static void PrintUsage(const std::string& programName);
static void PrintSystem(const std::string& prefix, const ProblemState& state);
static bool DoGaussianElimination(ProblemState* pState);

const int NUM_EXPECTED_ARGS = 2;
int main(int argc, char** ppArgv)
{
    if (argc < NUM_EXPECTED_ARGS + 1)
    {
        PrintUsage(ppArgv[0]);
        return -1;
    }

    const int MatrixSize = atoi(ppArgv[1]);
    const int NumberOfMatrices = atoi(ppArgv[2]);

    ProblemState state;
    state.MatrixSize = MatrixSize;
    state.spMatrix.reset(new double[MatrixSize * MatrixSize]);
    state.spB.reset(new double[MatrixSize]);

    int i = 0;
    do
    {
        InitializeSystem(&state);
        PrintSystem("Initial System:", state);

        //
        // Continue until we have a valid matrix.
        //
        if (!DoGaussianElimination(&state)) { continue; }

        PrintSystem("Solved System:", state);
        ++i;
    } while (i < NumberOfMatrices);

    return 0;
}

static void PrintUsage(const std::string& programName)
{
    std::cout << "Usage: " << programName << " <square matrix size> <number of matrices>" << std::endl;
}

static double& GetElement(double* pMatrix, int r, int c, int stride)
{
    return pMatrix[c + r * stride];
}

static void PrintSystem(const std::string& prefix, const ProblemState& state)
{
    std::cout << prefix << std::endl;

    for (size_t j = 0; j < state.MatrixSize; j++)
    {
        for (size_t i = 0; i < state.MatrixSize; i++)
        {
            const double& element = 
                GetElement(
                    state.spMatrix.get(), i, j, state.MatrixSize
                );

            std::cout << element << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    for (size_t j = 0; j < state.MatrixSize; j++)
    {
        std::cout << state.spB[j] << "\n";
    }

    std::cout << std::endl;
}

static bool DoGaussianElimination(ProblemState* pState)
{
    for (size_t r = 0; r < pState->MatrixSize; r++)
    {
        MaybePivot(pState, r);
        if (!ForwardSubstitution(pState, r))
        {
            return false;
        }
    }

    PrintSystem("After forward sub.", *pState);

    for (int r = pState->MatrixSize - 1; r >= 0; r--)
    {
        if (!BackSubstitution(pState, static_cast<size_t>(r)))
        {
            return false;
        }
    }

    PrintSystem("After backwards sub.", *pState);

    return true;
}
