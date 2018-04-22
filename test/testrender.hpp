#pragma once

#include "render/shader.hpp"
#include "scene/trianglemesh.hpp"


namespace Base
{
    void testRenderInit(AppHandler* app)
    {
        Scene* scene = new Scene();
        scene->objects.add(new Plane({ 1000, 1000 }, (Image*)appHandler->resHandler->find("images/avatar.png")->loaded));
        //scene->objects.add(new Sphere(10));
        app->currentScene = scene;
    }

    void testRender(AppHandler* app)
    {
        drawBegin();
        Mat4f projMat = app->currentScene->camera.getMatrix(app->mainWindow->ratio);
        for (Object* obj : app->currentScene->objects)
            obj->render(app->drawingShader, projMat);
    }
}