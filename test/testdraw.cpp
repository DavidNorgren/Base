#include "test.hpp"


void Base::TestApp::testDraw()
{
    setRenderTarget(mainWindow);
    drawBegin();
    drawText("FPS: " + toString(mainWindow->getFps()), { 10, 10 }, Colors::YELLOW);
    drawImage(debugSurface, { 10, mainWindow->getSize().height - debugSurface->getSize().height - 10 });
}