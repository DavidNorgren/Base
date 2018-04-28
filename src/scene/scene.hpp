#pragma once

#include "resource/resource.hpp"
#include "resource/shader.hpp"
#include "scene/object.hpp"
#include "scene/camera.hpp"
#include "util/data/list.hpp"

namespace Base
{
    class Scene
    {
      public:
        Scene() {}
        void render(Shader* shader);
        
        List<Object*> objects;
        Camera camera;
    };
}