#pragma once

#include <algorithm>
#include <math.h>

typedef unsigned int uint;
typedef unsigned char uchar;


namespace Base
{
    
    inline float min(float x, float y)
    {
        return x < y ? x : y;
    }


    inline float max(float x, float y)
    {
        return x > y ? x : y;
    }


    inline long mod(long a, long b)
    {
        return (a % b + b) % b;
    }


    inline float clamp(float x, float mi, float ma)
    {
        return min(ma, max(mi, x));
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