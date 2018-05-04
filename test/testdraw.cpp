#include "test.hpp"


void Base::TestApp::testDraw()
{
    setRenderTarget(mainWindow);
    drawBegin();
    drawText("FPS: " + toString(mainWindow->getFps()), { 10, 10 }, Colors::YELLOW);

    static bool showDebug = false;
    if (keyPressed(GLFW_KEY_Z))
        showDebug = !showDebug;

    if (showDebug)
    {
        ScreenPos debugPos = { 10, mainWindow->getSize().height - debugSurface->getSize().height - 10 };
        float scale = debugSurface->getSize().height / 2048.f;
        drawImage(currentScene->lights[0]->getShadowMaps()[0], debugPos, Colors::WHITE, 0.f, Vec2f(scale));
        drawImage(currentScene->lights[0]->getShadowMaps()[1], debugPos + Vec2i( 2048.f * scale + 10.f, 0 ), Colors::WHITE, 0.f, Vec2f(scale));
        drawImage(currentScene->lights[0]->getShadowMaps()[2], debugPos + Vec2i(2048.f * scale * 2.f + 20.f, 0 ), Colors::WHITE, 0.f, Vec2f(scale));
        //drawImage(debugSurface, debugPos);
        //drawBox({ debugPos, debugSurface->getSize() }, Colors::YELLOW, true, 2);
    }
}