#pragma once

#include "resource/shader.hpp"
#include "scene/camera.hpp"
#include "scene/object.hpp"
#include "scene/light.hpp"
#include "util/data/list.hpp"

namespace Base
{
    /* A 3D scene. */
    class Scene
    {
      public:
        Scene() { camera = new Camera(); }

        EXPORT void render(Shader* shader, Camera* camera = nullptr, Camera* cullCamera = nullptr);
        EXPORT Object* findObject(const string& name) const;
        
        Map<string, Object*> objectNames;
        List<Object*> objects;
        List<Light*> lights;
        Camera* camera;
        Color background;
    };
}