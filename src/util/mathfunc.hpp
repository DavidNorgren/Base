#pragma once

#include <algorithm>
#include <cmath>


namespace Base
{
    constexpr float PI = 3.14159265;

    template<typename T> inline T min(T x, T y)
    {
        return x < y ? x : y;
    }

    template<typename T> inline T max(T x, T y)
    {
        return x > y ? x : y;
    }

    template<typename T> inline T mod(T a, T b)
    {
        return (a % b + b) % b;
    }

    template<typename T> inline T clamp(T x, T lower, T upper)
    {
        return max(lower, min(upper, x));
    }
    
    inline float dsin(float a)
    {
        return std::sin(a * (PI / 180.f));
    }
    
    inline float dcos(float a)
    {
        return std::cos(a * (PI / 180.f));
    }
    
    inline float dtan(float a)
    {
        return std::tan(a * (PI / 180.f));
    }
}