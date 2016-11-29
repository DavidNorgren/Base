#pragma once


namespace Base
{

    /* Used to represent a position on the screen. */
    struct ScreenPos
    {
        int x, y;
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