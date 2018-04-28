#include "common.hpp"
#include "scene/scene.hpp"
#include "render/drawfunc.hpp"
#include "apphandler.hpp"


void Base::Scene::render(Shader* shader)
{
    shader->select();
    Mat4f projMat = camera.getMatrix(appHandler->mainWindow->getRatio());
    for (Object* obj : objects)
        obj->render(shader, projMat);
}