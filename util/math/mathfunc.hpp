#pragma once

#include <algorithm>
#include <math.h>

typedef unsigned int uint;
typedef unsigned char uchar;


namespace Base
{
    
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


    template<typename T> inline T clamp(T x, T mi, T ma)
    {
        return max(mi, min(ma, x));
    }


    inline float frand()
    {
        return (float)rand() / RAND_MAX;
    }


    inline float frand(float a, float b)
    {
        return a + frand() * (b - a);
    }
    
    
    inline float dsin(float a)
    {
        return sin(a * (M_PI / 180.f));
    }

    
    inline float dcos(float a)
    {
        return cos(a * (M_PI / 180.f));
    }

    
    inline float dtan(float a)
    {
        return tan(a * (M_PI / 180.f));
    }

}