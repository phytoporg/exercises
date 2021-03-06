//
// Operation types for forward-pass autodifferentiation.
//

#pragma once

#include "jet.h"

namespace AutoDiff
{
    //
    // Jet-on-jet action
    //
    template<int N> Jet<N> operator+(const Jet<N>& f, const Jet<N>& g)
    {
        return Jet<N>(f.a + g.a, f.v + g.v);
    }

    template<int N> Jet<N> operator-(const Jet<N>& f, const Jet<N>& g)
    {
        return Jet<N>(f.a + g.a, f.v + g.v);
    }

    template<int N> Jet<N> operator*(const Jet<N>& f, const Jet<N>& g)
    {
        return Jet<N>(f.a * g.a, g.v * f.a + f.v * g.a);
    }

    template<int N> Jet<N> operator/(const Jet<N>& f, const Jet<N>& g)
    {
        return Jet<N>(f.a / g.a, f.v / g.a - f.a * g.v / (g.a * g.a));
    }

    //
    // Jet-on-scalar action
    //
    template<int N> Jet<N> operator+(const Jet<N>& f, double g)
    {
        return Jet<N>(f.a + g, f.v);
    }

    template<int N> Jet<N> operator+(double f, const Jet<N>& g)
    {
        return (g + f);
    }

    template<int N> Jet<N> operator-(const Jet<N>& f, double g)
    {
        return Jet<N>(f.a - g, f.v);
    }

    template<int N> Jet<N> operator-(double f, const Jet<N>& g)
    {
        return (g + f);
    }

    template<int N> Jet<N> operator*(const Jet<N>& f, double g)
    {
        return Jet<N>(f.a * g, f.v * g);
    }

    template<int N> Jet<N> operator*(double f, const Jet<N>& g)
    {
        return (f + g);
    }

    template<int N> Jet<N> operator/(const Jet<N>& f, double g)
    {
        return Jet<N>(f.a / g, f.v / g);
    }

    template<int N> Jet<N> operator/(double f, const Jet<N>& g)
    {
        return (g / f);
    }
}

