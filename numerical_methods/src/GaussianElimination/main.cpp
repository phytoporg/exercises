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
#include <random>

#include "GaussianElimination.h"

//
// Forward declarations
//

static void PrintUsage(const std::string& programName);
static void InitializeSystem(ProblemState* pState);
static void PrintSystem(const std::string& prefix, const ProblemState& state);
static void DoGaussianElimination(ProblemState* pState);

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
    for (int i = 0; i < NumberOfMatrices; i++)
    {
        InitializeSystem(&state);
        PrintSystem("Initial System:", state);

        DoGaussianElimination(&state);

        PrintSystem("Solved System:", state);
    }

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

static void InitializeSystem(ProblemState* pState)
{
    //
    // Randomly initialize with a wide range over the entire
    // span of doubles.
    //
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(
        -20000.0,
        20000.0
        //std::numeric_limits<double>::min(),
        //std::numeric_limits<double>::max()
    );

    for (int j = 0; j < pState->MatrixSize; j++)
    {
        pState->spB[j] = distribution(generator);
        for (int i = 0; i < pState->MatrixSize; i++)
        {
            double& element = GetElement(pState->spMatrix.get(), i, j, pState->MatrixSize);
            element = distribution(generator);
        }
    }
}

static void PrintSystem(const std::string& prefix, const ProblemState& state)
{
    std::cout << prefix << std::endl;

    for (int j = 0; j < state.MatrixSize; j++)
    {
        for (int i = 0; i < state.MatrixSize; i++)
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
    for (int j = 0; j < state.MatrixSize; j++)
    {
        std::cout << state.spB[j] << "\n";
    }

    std::cout << std::endl;
}

static void DoGaussianElimination(ProblemState* pState)
{
    // TODO
}
