#pragma once

#include "util/data/list.hpp"
#include "scene/object.hpp"
#include "scene/camera.hpp"

namespace Base
{
    class Scene
    {
      public:
        List<Object*> objects;
        Camera camera;
    
      private:
    };
}