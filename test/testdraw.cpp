#include "test.hpp"


void Base::TestApp::testDraw()
{
    setRenderTarget(mainWindow);
    drawBegin();
    drawText("FPS: " + toString(mainWindow->getFps()), { 10, 10 }, Colors::YELLOW);

    ScreenPos debugPos = { 10, mainWindow->getSize().height - debugSurface->getSize().height - 10 };
    drawImage(debugSurface, debugPos);
    drawBox({ debugPos, debugSurface->getSize() }, Colors::YELLOW, true, 2);
}