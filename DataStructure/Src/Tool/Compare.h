#pragma once

namespace my_stl
{
    template<typename T>
    struct less
    {
        bool operator()(T const& left, T const& right)
        {
            return left < right;
        }
    };

    template<typename T>
    struct greater
    {
        bool operator()(T const& left, T const& right)
        {
            return left > right;
        }
    };
}