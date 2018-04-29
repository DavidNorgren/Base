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
        Object* rotate(const Vec3f& angles);
        Object* rotateX(float angle);
        Object* rotateY(float angle);
        Object* rotateZ(float angle);

        void buildMatrix();
        void resetTransform();

      protected:
        Vec3f pos, rot, sca;
        Mat4f matrix;
    };
}