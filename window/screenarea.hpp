#pragma once

#include "window/screenpos.hpp"


namespace Base
{

    /* Used to define an area on the screen. */
    struct ScreenArea
    {
        ScreenPos pos;
        int width, height;
    };

}