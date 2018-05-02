#include "test.hpp"
#include "resource/shader.hpp"
#include "resource/testscene.hpp"
#include "input/mousefunc.hpp"


const string testScene  = "scenes/checker.testscene";
const string testShader = "shaders/shadows.glsl";
Base::Light* sceneLight;
Base::Camera debugCamera;

void Base::TestApp::testSceneInit()
{
    debugSurface = new Surface({ 350, 250 });
    debugCamera.setPosition({ -100.f, 100.f, 100.f });
    debugCamera.buildMatrix(debugSurface->getRatio());

    // Setup camera
    camAngleXZ = camGoalAngleXZ = 90.f;
    camAngleY  = camGoalAngleY  = 20.f;
    camZoom    = camGoalZoom    = 100.f;
    camMove    = false;

    ((Shader*)resHandler->get("shaders/shadows.glsl"))->setSetupFunc([&](GLuint glProgram, const Mat4f& matM)
    {
        GLint uDepthSampler = glGetUniformLocation(glProgram, "uDepthSampler");
        GLint uLightDir = glGetUniformLocation(glProgram, "uLightDir");
        GLint uMatDepthBiasMVP = glGetUniformLocation(glProgram, "uMatDepthBiasMVP");

        // Send in depth
        const List<ShadowMap*>& maps = sceneLight->getShadowMaps();
        List<Mat4f> mats;
        
        for (uint i = 0; i < maps.size(); i++)
        {
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_2D, maps[i]->getGlTexture());
            glUniform1i(uDepthSampler, 1 + i);

            mats.add(maps[i]->getMatrix() * matM);
        }

        // Send in depth matrices
        glUniformMatrix4fv(uMatDepthBiasMVP, 1, GL_FALSE, mats[0].e);

        // Light direction for shading
        glUniform3fv(uLightDir, 1, (float*)&sceneLight->getDir());
    });
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

    // Move light
    sceneLight->translate({
        (keyDown(GLFW_KEY_D) - keyDown(GLFW_KEY_A)) * 10.f,
        (keyDown(GLFW_KEY_Q) - keyDown(GLFW_KEY_E)) * 10.f,
        (keyDown(GLFW_KEY_S) - keyDown(GLFW_KEY_W)) * 10.f
    });
}

void Base::TestApp::testSceneRender()
{
    // Load the scene from JSON during start or when modified
    currentScene = (TestScene*)resHandler->get(testScene);
    sceneLight = currentScene->lights[0];

    // Animate some objects
    float d = mainWindow->getFrameDelay();
    currentScene->findObject("teapot")->rotateY(d)->buildMatrix();
    currentScene->findObject("box")->rotateX(d)->rotateY(d)->rotateZ(d)->buildMatrix();

    // Smoothen the camera motion
    camAngleXZ += (camGoalAngleXZ - camAngleXZ) / 5.f;
    camAngleY  += (camGoalAngleY  - camAngleY)  / 5.f;
    camZoom    += (camGoalZoom    - camZoom)    / 5.f;

    // Camera rotates in a circle around the scene origin
    currentScene->camera->setPosition({
        dcos(camAngleXZ) * dcos(camAngleY) * camZoom,
        dsin(camAngleY) * camZoom,
        dsin(camAngleXZ) * dcos(camAngleY) * camZoom
    });
    currentScene->camera->buildMatrix(mainWindow->getRatio());

    // Debug window
    setRenderTarget(debugSurface);
    currentScene->render((Shader*)resHandler->get(testShader), &debugCamera);
    if (currentScene->camera->frustumModel)
        currentScene->camera->frustumModel->render(((Shader*)resHandler->get(testShader)), Mat4f::identity(), debugCamera.getViewProjection());

    // Render to shadow maps
    sceneLight->startShadowMapPass(currentScene->camera);
    for (ShadowMap* map : sceneLight->getShadowMaps())
    {
        setRenderTarget(map);
        currentScene->render((Shader*)resHandler->get("shaders/depth.glsl"), sceneLight);
    }

    // Render some goodness
    setRenderTarget(mainWindow);
    currentScene->render((Shader*)resHandler->get(testShader));
}