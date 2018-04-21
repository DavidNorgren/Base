#pragma once


namespace Base
{
    inline float frand()
    {
        return (float)rand() / RAND_MAX;
    }

    inline float frand(float a, float b)
    {
        return a + frand() * (b - a);
    }
}