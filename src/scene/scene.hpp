#pragma once

#include "resource/resource.hpp"
#include "resource/shader.hpp"
#include "scene/object.hpp"
#include "scene/light.hpp"
#include "scene/camera.hpp"
#include "util/data/list.hpp"

namespace Base
{
    class Scene
    {
      public:
        Scene() {}

        /* Render the scene from the camera's POV. */
        void render(Shader* shader) const;

        /* Render the scene from a custom POV. */
        void render(Shader* shader, const Mat4f& matVP) const;
        
        List<Object*> objects;
        List<Light*> lights;
        Camera camera;
    };
}