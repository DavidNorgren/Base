#pragma once

#include "util/data/mat4.hpp"
#include "util/data/vec3.hpp"

namespace Base
{
    class Camera
    {
      public:
        EXPORT Mat4f getMatrix(float ratio);
        
        Vec3f xaxis, yaxis, zaxis, pos;
        float fov;
    };
}