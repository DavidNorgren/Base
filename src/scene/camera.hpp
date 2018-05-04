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
        void setFov(float angle)                { fov = angle; }
        void setPosition(const Vec3f& position) { pos = position; }
        Camera* translate(const Vec3f& vec)     { pos += vec; }

        float getFovV() const  { return fov; }
        float getFovH() const  { return fov * ratio; }
        float getZNear() const { return zNear; }
        float getZFar() const  { return zFar; }
        float getRatio() const { return ratio; }

        virtual void buildMatrix(float ratio);
        const Mat4f& getView() const { return matV; }
        const Mat4f& getProjection() const { return matP; }
        const Mat4f& getViewProjection() const { return matVP; }

        // DEBUG
        TriangleMesh* frustumMesh = nullptr;
        Material* frustumMaterial = nullptr;
        Model* frustumModel = nullptr;

      protected:
        Vec3f pos;
        float fov = 90.f;
        float ratio = 1.f;
        float zNear = 1.f, zFar = 10000.f;
        Mat4f matV, matP, matVP;
    };
}