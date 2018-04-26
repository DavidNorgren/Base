#pragma once

#include "resource/shader.hpp"
#include "resource/model.hpp"
#include "scene/trianglemesh.hpp"


namespace Base
{
    void testRenderInit(AppHandler* app)
    {
        Scene* scene = new Scene();

        try
        {
            scene->objects.add(
                new Plane({ 1000, 1000 }, (Image*)appHandler->resHandler->get("images/grid.png"), { 20.f })
            );
           // scene->objects.add(
           //     appHandler->resHandler->get("models/bunny.obj")
           // );
        }
        catch (ModelFileException e)
        {
            cout << "Model file error: " << e.what() << endl;
        }

        //scene->objects.add(new Sphere(10));
        app->currentScene = scene;
    }

    void testRender(AppHandler* app)
    {
        static float d = 0.f;
        app->currentScene->camera.pos = { dcos(d) * 100.f, 50.f, dsin(d) * 100.f };
        d += 0.5f * app->mainWindow->getFrameDelay();
        
        drawBegin();
        Mat4f projMat = app->currentScene->camera.getMatrix(app->mainWindow->getRatio());
        for (Object* obj : app->currentScene->objects)
            obj->render(app->drawingShader, projMat);
    }
}