#pragma once

#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "resource/shader.hpp"
#include "scene/model.hpp"
#include "scene/volume.hpp"


namespace Base
{
    /* An object in the scene with a visual representation. */
    class Object
    {
      public:
        Object(const string& name = "");
        Object(Model* model, const string& name = "");
        
        virtual void render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const;

        Object* translate(const Vec3f& translate);
        Object* scale(const Vec3f& scale);
        Object* rotate(const Vec3f& angles);
        Object* rotateX(float angle);
        Object* rotateY(float angle);
        Object* rotateZ(float angle);

        void buildMatrix();
        void resetTransform();

        const BoundingBox& getBoundingBox() const { return box; }

      protected:
        string name;
        Vec3f pos, rot, sca;
        Mat4f matrix;
        Model* model;
        BoundingBox box;
    };
}