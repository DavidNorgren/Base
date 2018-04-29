#include "common.hpp"
#include "scene/scene.hpp"
#include "render/drawfunc.hpp"
#include "apphandler.hpp"


void Base::Scene::render(Shader* shader) const
{
    render(shader, camera.getMatrix(appHandler->mainWindow->getRatio()));
}

void Base::Scene::render(Shader* shader, const Mat4f& matVP) const
{
    shader->select();
    glClear(GL_DEPTH_BUFFER_BIT);
    for (Object* obj : objects)
        obj->render(shader, Mat4f::identity(), matVP);
}