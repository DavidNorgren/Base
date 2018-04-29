#include "test.hpp"
#include "resource/shader.hpp"
#include "resource/model.hpp"
#include "resource/testscene.hpp"
#include "input/mousefunc.hpp"


void Base::TestApp::testSceneInit()
{
    // Setup camera
    camAngleXZ = camGoalAngleXZ = 90.f;
    camAngleY  = camGoalAngleY  = 20.f;
    camZoom    = camGoalZoom    = 100.f;
    camMove    = false;
}

void Base::TestApp::testSceneInput()
{
    // Left click rotates
    if (camMove)
    {
        if (mouseLeftDown())
        {
            camGoalAngleXZ += mouseMove().x * 0.25f;
            camGoalAngleY  += mouseMove().y * 0.25f;
            camGoalAngleY   = clamp(camGoalAngleY, -89.f, 89.f);
        }
        else
            camMove = false;
    }
    else if (mouseLeftPressed())
        camMove = true;
    
    // Scroll zooms (sideways scroll will also rotate)
    camGoalAngleXZ += mouseScroll().x * 5.f;
    camGoalZoom    *= 1.f - 0.1f * mouseScroll().y;
    camGoalZoom     = clamp(camGoalZoom, 10.f, 2000.f);
}

void Base::TestApp::testSceneRender()
{
    // Load the scene from JSON during start or when modified
    currentScene = (TestScene*)resHandler->get("scenes/checker.testscene");

    // Animate some objects
    float d = mainWindow->getFrameDelay();
    ((Model*)resHandler->get("models/teapot/teapot.obj"))->rotateY(d)->buildMatrix();

    // Camera rotates in a circle around the scene origin
    currentScene->camera.pos = {
        dcos(camAngleXZ) * dcos(camAngleY) * camZoom,
        dsin(camAngleY) * camZoom,
        dsin(camAngleXZ) * dcos(camAngleY) * camZoom
    };

    // Smoothen the motion
    camAngleXZ += (camGoalAngleXZ - camAngleXZ) / 5.f;
    camAngleY  += (camGoalAngleY  - camAngleY)  / 5.f;
    camZoom    += (camGoalZoom    - camZoom)    / 5.f;
    
    // Render some goodness
    currentScene->render((Shader*)resHandler->get("shaders/test.glsl"));
}