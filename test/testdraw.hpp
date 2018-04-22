#pragma once

#include "render/drawfunc.hpp"


namespace Base
{
    void testDraw(AppHandler* app)
    {
        drawBegin();
        drawText("FPS: " + toString(app->mainWindow->fps), { 10, 10 }, Colors::YELLOW);
    }
}