#pragma once

#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "resource/shader.hpp"

namespace Base
{
    /* An object in the scene with a visual representation. */
    class Object
    {
      public:
        Object();
        virtual void render(Shader* shader, const Mat4f& projMat) const = 0;

        Object* translate(const Vec3f& translate);
        Object* scale(const Vec3f& scale);
        Object* rotateZ(const float angle);
        void resetTransform();

      protected:
        void buildMatrix();
        Vec3f pos, rot, sca;
        Mat4f matrix;
    };
}