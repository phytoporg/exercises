//
// Jet dual type for convenient autodifferentiation.
//
// Inspired by ceres.
//
// https://en.wikipedia.org/wiki/Jet_(mathematics)
//

#pragma once

#include "vector.h"

namespace AutoDiff
{
    template<int N>
    struct Jet
    {
        Jet() : a(0.0), v(0.0) {}

        Jet(double _a)
            : a(_a), v(0.0) {}

        Jet(double _a, const Neo::Vector<double, N>& _v)
            : a(_a), v(_v) {}

        double                 a;
        Neo::Vector<double, N> v;
    };
}

