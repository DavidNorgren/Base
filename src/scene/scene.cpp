#include "common.hpp"
#include "scene/scene.hpp"
#include "render/renderfunc.hpp"
#include "apphandler.hpp"


EXPORT void Base::Scene::render(Shader* shader, Camera* camera)
{
    shader->select();
    glClear(GL_DEPTH_BUFFER_BIT);

    if (!camera)
        camera = this->camera;
    
    camera->buildMatrix(getRenderTarget()->getRatio());

    for (Object* obj : objects)
        obj->render(shader, Mat4f::identity(), camera->getViewProjection());
}