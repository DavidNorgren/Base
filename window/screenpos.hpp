#pragma once

#include "util/math/mathfunc.hpp"


namespace Base
{

    /* Used to represent a position on the screen. */
    struct ScreenPos
    {
        int x, y;

        static inline int distance(ScreenPos a, ScreenPos b)
        {
            return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
        }
    };


    //// Binary operators ////

    inline ScreenPos operator + (const ScreenPos& a, const ScreenPos& b)
    {
        return { a.x + b.x, a.y + b.y };
    }


    inline void operator += (ScreenPos& a, const ScreenPos& b)
    {
        a.x += b.x;
        a.y += b.y;
    }


    inline ScreenPos operator - (const ScreenPos& a, const ScreenPos& b)
    {
        return { a.x - b.x, a.y - b.y };
    }


    inline void operator -= (ScreenPos& a, const ScreenPos& b)
    {
        a.x -= b.x;
        a.y -= b.y;
    }

}