#include "test.hpp"


void Base::TestApp::testDraw()
{
    setRenderTarget(mainWindow);
    drawBegin();
    drawText("FPS: " + toString(mainWindow->getFps()), { 10, 10 }, Colors::YELLOW);

    ScreenPos debugPos = { 10, mainWindow->getSize().height - debugSurface->getSize().height - 10 };
    float scale = debugSurface->getSize().height / 1024.f;
    drawImage(currentScene->lights[0]->getShadowMaps()[0], debugPos, Colors::WHITE, 0.f, Vec2f(scale));
    //drawImage(debugSurface, debugPos);
    //drawBox({ debugPos, debugSurface->getSize() }, Colors::YELLOW, true, 2);
}