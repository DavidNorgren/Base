#include "test.hpp"


void Base::TestApp::testDraw()
{
    setRenderTarget(mainWindow);
    drawBegin();
    drawText("FPS: " + toString(mainWindow->getFps()) + "\n", { 10, 10 }, Colors::YELLOW);
}