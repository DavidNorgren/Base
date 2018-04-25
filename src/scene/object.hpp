#pragma once

#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "render/shader.hpp"
#include "render/image.hpp"

namespace Base
{
    /* An object in the scene with a visual representation. */
    class Object
    {
      public:
        virtual void render(Shader* shader, Mat4f projMat) = 0;

      protected:
        Vec3f pos, rot, scale;
        Image* texture;
    };
}