#pragma once

#include "scene/camera.hpp"
#include "render/color.hpp"
#include "render/rendertarget.hpp"
#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    constexpr float LIGHT_CASCADES[] = { 0.f, 0.2f, 0.5f, 1.f };

    /* The format of the shadow mapping for this light. */
    enum class ShadowMapFormat
    {
        SINGLE,
        CASCADE,
        CUBE
    };

    /* A light in the scene. */
    class ShadowMap;
    class Light : public Camera
    {
      public:
        Light();
        ~Light();

        /* Color */
        void setColor(const Color& color) { this->color = color; }
        const Color& getColor() const     { return color; }
        
        /* Position in the scene. */
        void setPosition(const Vec3f& position);
        Light* translate(const Vec3f& translate);
        const Vec3f& getDir() const { return dir; }

        /* Builds matrix for rendering. */
        void buildMatrix(float ratio) override;

        /* Starts a pass for shading mapping. */
        void startShadowMapPass(const Camera* sceneCamera);

        /* Returns references to the generated shadowmaps. */
        const List<ShadowMap*>& getShadowMaps() const { return shadowMaps; }

      private:
        Vec3f dir;
        Color color;

        List<ShadowMap*> shadowMaps;
        Size3Df orthoSize = { 200.f, 200.f, 1000.f };
        Mat4f matBiasVP;
    };

    /* A depth map for shadows. */
    class ShadowMap : public RenderTarget
    {
      public:
        ShadowMap(Size2Di size);
        const Mat4f& getMatrix() const { return matBiasVP; };

      friend class Light;
      protected:
        Mat4f matBiasVP;
    };
}