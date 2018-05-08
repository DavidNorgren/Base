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
        EXPORT Object(const string& name = "");
        EXPORT Object(Model* model, const string& name = "");
        
        EXPORT virtual void render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const;

        EXPORT Object* translate(const Vec3f& translate);
        EXPORT Object* scale(const Vec3f& scale);
        EXPORT Object* rotate(const Vec3f& angles);
        EXPORT Object* rotateX(float angle);
        EXPORT Object* rotateY(float angle);
        EXPORT Object* rotateZ(float angle);

        EXPORT Object* buildMatrix();
        EXPORT void resetTransform();

        const BoundingBox& getBoundingBox() const { return box; }
        bool getOcclude() const { return occlude; }
        void setOcclude(bool occlude) { this->occlude = occlude; }

      protected:
        string name;
        Vec3f pos, rot, sca;
        Mat4f matrix;
        Model* model;
        BoundingBox box;
        bool occlude = true;
    };
}