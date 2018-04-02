#pragma once

#include "window/screenpos.hpp"

#include <iostream>

namespace Base
{
    /* Used to define an area on the screen. */
    struct ScreenArea
    {
        ScreenPos pos;
        int width, height;

        static inline ScreenArea intersection(ScreenArea a, ScreenArea b)
        {
            ScreenPos start = { max(a.pos.x, b.pos.x), max(a.pos.y, b.pos.y) };
            ScreenPos end = { min(a.pos.x + a.width, b.pos.x + b.width), min(a.pos.y + a.height, b.pos.y + b.height) };
            return {
                start,
                max(0, end.x - start.x),
                max(0, end.y - start.y)
            };
        }
    };
    
    //// Unary operators ////

    inline std::ostream& operator << (std::ostream& cout, const ScreenArea& a)
    {
        return std::cout << "({ " << a.pos.x << "," << a.pos.y << " }," << a.width << "," << a.height << ")" << std::endl;
    }
}