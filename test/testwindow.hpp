#pragma once

#include "render/drawfunc.hpp"

namespace Base
{
    void testWindow(ResourceHandler* resHandler)
    {
        drawBegin();
        drawImage("images/avatar.png", { 0, 0 });
        drawText("Hello World!", { 200, 40 }, Colors::GREEN);
    }
}