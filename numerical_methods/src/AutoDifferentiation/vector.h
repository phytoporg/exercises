//
// TODO: factor out into separate library
//

#pragma once

#include <cstdlib>
#include <cstring>

namespace Neo
{
    template<typename T, int N>
    class Vector
    {
    public:
        Vector<T, N>() = default;

        //
        // Very safe and good copy that could never ever go wrong.
        //
        Vector<T, N>(T v) { 
            for(int i = 0; i < N; ++i) { m_v[i] = v; }
        }

        Vector<T, N>(T* pV) {
            memcpy(
                static_cast<void*>(m_v),
                static_cast<const void*>(pV),
                sizeof(T) * N);
        }

        Vector<T, N>(const Vector<T, N>& other) { 
            memcpy(
                static_cast<void*>(m_v),
                static_cast<const void*>(other.m_v), 
                sizeof(T) * N);
        }

        Vector<T, N> operator+(const Vector<T, N>& other) const
        {
            Vector<T, N> ret;
            for(size_t i = 0; i < N; ++i)
            {
                ret.m_v[i] = m_v[i] + other.m_v[i];
            }

            return ret;
        }

        Vector<T, N> operator-(const Vector<T, N>& other) const
        {
            Vector<T, N> ret;
            for(size_t i = 0; i < N; ++i)
            {
                ret.m_v[i] = m_v[i] - other.m_v[i];
            }

            return ret;
        }

        Vector<T, N> operator*(const T& c) const
        {
            Vector<T, N> ret;
            for(size_t i = 0; i < N; ++i)
            {
                ret.m_v[i] = m_v[i] * c;
            }

            return ret;
        }

        Vector<T, N> operator/(const T& c) const
        {
            Vector<T, N> ret;
            for(size_t i = 0; i < N; ++i)
            {
                ret.m_v[i] = m_v[i] / c;
            }

            return ret;
        }

        T operator[](size_t i) const
        {
            return m_v[i];
        }

        T& operator[](size_t i)
        {
            return m_v[i];
        }

    private:
        T m_v[N];
    };
}
