#include <iostream>
#include "jet.h"
#include "ops.h"

using namespace AutoDiff;

int main()
{
    Jet<1> x(10);
    auto f = x * x;
    double d = f.v[0];

    std::cout << "Derivative of f(x^2) at 10 is " << d << std::endl;
    return 0;
}
