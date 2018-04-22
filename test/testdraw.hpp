#pragma once

#include "render/drawfunc.hpp"


namespace Base
{
    void testDraw(AppHandler* app)
    {
        drawBegin();
        drawImage("images/avatar.png", { 0, 0 });
        drawText("Hello world!", { 200, 40 }, Colors::GREEN);

        for (float d = 0.f; d < 360.f; d+=20.f)
        {
            drawLine(
                Vec2i(400, 400) + Vec2i((int)(100.f * dcos(d)), (int)(100.f * dsin(d))),
                Vec2i(400, 400) + Vec2i((int)(100.f * dcos(d - 20.f)), (int)(100.f * dsin(d - 20.f))),
                Colors::RED, 100
            );
        }
    }
}