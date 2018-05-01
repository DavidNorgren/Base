#pragma once

#include "util/data/mat4.hpp"
#include "util/data/vec3.hpp"
#include "util/data/size3d.hpp"

#include "scene/model.hpp"


namespace Base
{
    class Camera
    {
      public:
        
        void setPosition(const Vec3f& position) { pos = position; }
        Camera* translate(const Vec3f& vec)     { pos += vec; }
        virtual void buildMatrix(float ratio);

        const Mat4f& getViewProjection() const { return matVP; }

        TriangleMesh* frustumMesh = nullptr;
        Material* frustumMaterial = nullptr;
        Model* frustumModel = nullptr;

      protected:
        Vec3f xaxis, yaxis, zaxis, pos;
        float fov = 90.f;
        Mat4f matV, matP, matVP;
    };
}