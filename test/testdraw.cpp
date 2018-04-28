#include "test.hpp"


void Base::TestApp::testDraw()
{
    drawBegin();
    drawText("FPS: " + toString(mainWindow->fps), { 10, 10 }, Colors::YELLOW);
}