#include <chrono>
#include <iostream>
#include <random>

#include "jet.h"
#include "ops.h"

using namespace AutoDiff;
using namespace std; 

void 
NormalEquationsCostFunction(
    Jet<2> params[2],
    double x,
    double gt[2],
    Jet<2>* pResidual
)
{
    Jet<2> u[] = {params[0]*x*x, params[1]*x*x};
    *pResidual = 
        (u[0] - gt[0])*(u[0] - gt[0]) + 
        (u[1] - gt[1])*(u[1] - gt[1]);
}

int main()
{
    const size_t NumSamples{10};
    const size_t Dims{2};
    std::vector<double> xySamples(Dims * NumSamples);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> noiseDistribution (0.0, 0.3);

    std::uniform_real_distribution<double> paramsDistribution(0.0,10.0);
    const double params[] = {
        paramsDistribution(generator), 
        paramsDistribution(generator)
    };

    for (size_t i = 0; i < NumSamples; ++i)
    {
        const size_t Base{i * Dims};
        const auto gt1 = params[0] * static_cast<double>(i * i);
        const auto gt2 = params[1] * static_cast<double>(i * i); 

        xySamples[Base]     = gt1 + noiseDistribution(generator);
        xySamples[Base + 1] = gt2 + noiseDistribution(generator);
    }

    //
    // TODO: build full jacobian, do some non-linear least squares.
    //
    for (size_t i = 0; i < NumSamples; ++i)
    {
        Jet<2> params[2];
        params[0].v[0] = 1.0;
        params[1].v[1] = 1.0;

        double gt[2] = { xySamples[Dims*i], xySamples[Dims*i+1] };

        Jet<2> results;
        NormalEquationsCostFunction(
            params,
            static_cast<double>(i),
            gt,
            &results);

        //
        // Output jacobian for this sample.
        //
        for (size_t i = 0; i < 2; ++i)
        {
            cout << results.v[i] << " ";
        }
        cout << std::endl;
    }

    return 0;
}

