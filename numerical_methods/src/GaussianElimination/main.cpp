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

static void PrintUsage(const std::string& programName)
{
    std::cout << "Usage: " << programName << " <square matrix size> <number of matrices>" << std::endl;
}

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

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, std::numeric_limits<double>::max());

    std::unique_ptr<double[]> spMatrix(new double[MatrixSize * MatrixSize]);
    for (int i = 0; i < NumberOfMatrices; i++)
    {
        //InitializeMatrix(spMatrix.get(), MatrixSize);
        //PrintMatrix("Initial System:")

    }

    return 0;
}
