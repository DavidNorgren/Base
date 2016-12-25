#pragma once

#include "window/screenpos.hpp"


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
                end.x - start.x,
                end.y - start.y
            };
        }
    };

}