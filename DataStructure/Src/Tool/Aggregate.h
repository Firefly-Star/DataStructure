#pragma once

#include <limits>

namespace my_stl
{
    template<typename T>
    struct plus
    {
        T operator()(T const& left, T const& right)
        {
            return left + right;
        }
        T identity()
        {
            return static_cast<T>(0);
        }
    };

    template<typename T>
    struct min
    {
        T operator()(T const& left, T const& right)
        {
            return left < right ? left : right;
        }
        T identity()
        {
            return std::numeric_limits<T>::max();
        }
    };

    template<typename T>
    struct max
    {
        T operator()(T const& left, T const& right)
        {
            return left > right ? left : right;
        }
        T identity()
        {
            return std::numeric_limits<T>::min();
        }
    };

    template<typename T>
    struct multiply
    {
        T operator()(T const& left, T const& right)
        {
            return left * right;
        }
        T identity()
        {
            return static_cast<T>(1);
        }
    };
}