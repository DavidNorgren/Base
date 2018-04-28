#pragma once

#include <algorithm>
#include <cmath>


using std::abs;
using std::sqrt;

namespace Base
{
    constexpr float PI = 3.14159265;

    template<typename T> inline T min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template<typename T> inline T max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template<typename T> inline T mod(const T& a, const T& b)
    {
        return (a % b + b) % b;
    }

    template<typename T> inline T clamp(const T& a, const T& lower, const T& upper)
    {
        return max(lower, min(upper, a));
    }

    template<typename T> inline T approxEq(const T& a, const T& b, double absEpsilon = 1e-8, double relEpsilon = 1e-5)
    {
        double diff = abs(a - b);
        if (diff <= absEpsilon)
            return true;
    
        return diff <= ((abs(a) < fabs(b) ? fabs(b) : fabs(a)) * relEpsilon);
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