#pragma once

#include <cmath> // abs, fabs, sqrt, sin, cos, tan
#include <algorithm> // min, max
#include <limits> // numeric_limits


namespace Base
{
    using std::abs;
    using std::sqrt;
    using std::floor;
    using std::round;
    using std::ceil;
    using std::min;
    using std::max;
    using std::sin;
    using std::cos;
    using std::tan;

    constexpr float PI = 3.14159265;

    template<typename T> inline T minLimit()
    {
        return std::numeric_limits<T>::min();
    }
    
    template<typename T> inline T maxLimit()
    {
        return std::numeric_limits<T>::max();
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
        return sin(a * (PI / 180.f));
    }
    
    inline float dcos(float a)
    {
        return cos(a * (PI / 180.f));
    }
    
    inline float dtan(float a)
    {
        return tan(a * (PI / 180.f));
    }
}