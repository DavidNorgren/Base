#include "test.hpp"
#include "resource/shader.hpp"
#include "resource/testscene.hpp"
#include "input/mousefunc.hpp"


const string testScene  = "scenes/checker.testscene";
const string testShader = "shaders/csm.glsl";
Base::Light* sceneLight;

void Base::TestApp::testSceneInit()
{
    // Setup cameras
    camAngleXZ[0] = camGoalAngleXZ[0] = 90.f;
    camAngleY[0]  = camGoalAngleY[0]  = 20.f;
    camZoom[0]    = camGoalZoom[0]    = 100.f;
    camAngleXZ[1] = camGoalAngleXZ[1] = 45.f;
    camAngleY[1]  = camGoalAngleY[1]  = 45.f;
    camZoom[1]    = camGoalZoom[1]    = 200.f;
    camMove = -1;

    ((Shader*)resHandler->get("shaders/csm.glsl"))->setSetupFunc([&](GLuint glProgram, const Mat4f& matM)
    {
        GLint uLightDir = glGetUniformLocation(glProgram, "uLightDir");
        GLint uMatLightMVP = glGetUniformLocation(glProgram, "uMatLightMVP");
        GLint uMatLightBiasMVP = glGetUniformLocation(glProgram, "uMatLightBiasMVP");
        GLint uCascadeEndClipSpace = glGetUniformLocation(glProgram, "uCascadeEndClipSpace");

        // Send in depth
        const List<ShadowMap*>& maps = sceneLight->getShadowMaps();
        List<Mat4f> matsLightMVP, matsLightBiasMVP;
        List<float> cascadeEndClipSpaceDepth;
        
        for (uint i = 0; i < maps.size(); i++)
        {
            string name = "uDepthSampler[" + toString(i) + "]";
            GLint uDepthSampler = glGetUniformLocation(glProgram, &name[0]);
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_2D, maps[i]->getGlTexture());
            glUniform1i(uDepthSampler, 1 + i);

            matsLightMVP.add(maps[i]->getViewProjection() * matM);
            matsLightBiasMVP.add(maps[i]->getBiasViewProjection() * matM);
            cascadeEndClipSpaceDepth.add(maps[i]->getCascadeEndClipSpaceDepth());
        }

        // Send in light matrices
        glUniformMatrix4fv(uMatLightMVP,     maps.size(), GL_FALSE, matsLightMVP[0].e);
        glUniformMatrix4fv(uMatLightBiasMVP, maps.size(), GL_FALSE, matsLightBiasMVP[0].e);

        // Send in cascade splits
        glUniform1fv(uCascadeEndClipSpace, maps.size(), &cascadeEndClipSpaceDepth[0]);

        // Light direction for shading
        glUniform3fv(uLightDir, 1, (float*)&sceneLight->getDir());
    });
}

void Base::TestApp::testSceneInput()
{
    for (int i = 0; i < 2; i++)
    {
        bool mouseInCamera = (mousePos().y > (mainWindow->getSize().height / 2) * i && mousePos().y < (mainWindow->getSize().height / 2) * (i + 1));

        // Left click rotates
        if (camMove == i)
        {
            if (mouseLeftDown())
            {
                camGoalAngleXZ[i] += mouseMove().x * 0.25f;
                camGoalAngleY[i]  += mouseMove().y * 0.25f;
                camGoalAngleY[i]   = clamp(camGoalAngleY[i], -89.f, 89.f);
            }
            else
                camMove = -1;
        }
        else if (mouseLeftPressed() && mouseInCamera)
            camMove = i;
        
        if (mouseInCamera)
        {
            // Scroll zooms (sideways scroll will also rotate)
            camGoalAngleXZ[i] += mouseScroll().x * 5.f;
            camGoalZoom[i]    *= 1.f - 0.1f * mouseScroll().y;
            camGoalZoom[i]     = clamp(camGoalZoom[i], 10.f, 2000.f);
        }
    }

    // Move light
    sceneLight->translate({
        (keyDown(GLFW_KEY_D) - keyDown(GLFW_KEY_A)) * 4.f,
        (keyDown(GLFW_KEY_Q) - keyDown(GLFW_KEY_E)) * 4.f,
        (keyDown(GLFW_KEY_S) - keyDown(GLFW_KEY_W)) * 4.f
    });
}

void Base::TestApp::testSceneRender()
{
    // Load the scene from JSON during start or when modified
    currentScene = (TestScene*)resHandler->get(testScene);
    sceneLight = currentScene->lights[0];

    // Resize
    surfaces[0].resize({ mainWindow->getSize().width, mainWindow->getSize().height / 2});
    surfaces[1].resize({ mainWindow->getSize().width, mainWindow->getSize().height / 2});

    // Animate some objects
    float d = mainWindow->getFrameDelay();
    currentScene->findObject("teapot")->rotateY(d)->buildMatrix();
    currentScene->findObject("box")->rotateX(d)->rotateY(d)->rotateZ(d)->buildMatrix();

    // Smoothen the camera motion
    for (int i = 0; i < 2; i++)
    {
        camAngleXZ[i] += (camGoalAngleXZ[i] - camAngleXZ[i]) / 5.f;
        camAngleY[i]  += (camGoalAngleY[i]  - camAngleY[i])  / 5.f;
        camZoom[i]    += (camGoalZoom[i]    - camZoom[i])    / 5.f;

        // Camera rotates in a circle around the scene origin
        cameras[i].setPosition({
            dcos(camAngleXZ[i]) * dcos(camAngleY[i]) * camZoom[i],
            dsin(camAngleY[i]) * camZoom[i],
            dsin(camAngleXZ[i]) * dcos(camAngleY[i]) * camZoom[i]
        });
        cameras[i].buildMatrix(surfaces[i].getRatio());
    }

    // Render to shadow maps
    sceneLight->prepareShadowMaps(&cameras[0]);
    for (ShadowMap* map : sceneLight->getShadowMaps())
    {
        setRenderTarget(map);
        currentScene->render((Shader*)resHandler->get("shaders/depth.glsl"), map);
    }

    // Render some goodness
    setRenderTarget(&surfaces[0]);
    currentScene->render((Shader*)resHandler->get(testShader), &cameras[0]);

    // Debug window
    static bool showCamFrustum = true;
    static bool showOrthoBox = true;
    
    if (keyPressed(GLFW_KEY_2))
        showCamFrustum = !showCamFrustum;
    if (keyPressed(GLFW_KEY_3))
        showOrthoBox = !showOrthoBox;

    setRenderTarget(&surfaces[1]);
    debugShowLines = true;
    currentScene->render((Shader*)resHandler->get("shaders/texture.glsl"), &cameras[1], &cameras[0]);
    for (ShadowMap* map : sceneLight->getShadowMaps())
    {
        if (showCamFrustum)
            map->debugCamFrustum->render(((Shader*)resHandler->get("shaders/texture.glsl")), Mat4f::identity(), cameras[1].getViewProjection());
        if (showOrthoBox)
            map->debugOrthoBox->render(((Shader*)resHandler->get("shaders/texture.glsl")), Mat4f::identity(), cameras[1].getViewProjection());
    }
    debugShowLines = false;

    // Light position
    drawBegin();
    ScreenPos lightPos;
    if (cameras[1].getViewProjection().project(sceneLight->getPosition(), lightPos, surfaces[1].getSize()))
        drawImage((Sprite*)resHandler->get("images/bulb.png"), lightPos - Vec2i(32, 32), Colors::WHITE, 0.f, { 0.5f });
}