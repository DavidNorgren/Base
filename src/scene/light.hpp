#pragma once

#include "scene/camera.hpp"
#include "render/color.hpp"
#include "render/rendertarget.hpp"
#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"

namespace Base
{
    constexpr int   lightNumCascades = 1;
    constexpr float lightCascadeStart[] = { 0.f, 0.2f, 0.3f };

    /* The format of the shadow mapping for this light. */
    enum class ShadowMapFormat
    {
        SINGLE,
        CASCADE,
        CUBE
    };

    /* A depth map for shadows. */
    class ShadowMap : public RenderTarget
    {
      public:
        ShadowMap(Size2Di size);
    };

    /* A light in the scene. */
    class Light : public Camera
    {
      public:
        Light();
        ~Light();

        /* Color */
        void setColor(const Color& color) { this->color = color; }
        const Color& getColor() const { return color; }
        
        /* Position in the scene. */
        void setPosition(const Vec3f& position);
        Light* translate(const Vec3f& translate);
        const Vec3f& getDir() const { return dir; }

        void buildMatrix(float ratio) override;
        const Mat4f& getBiasViewProjection() const { return matBiasVP; }

        const List<ShadowMap*>& getShadowMaps() const { return shadowMaps; }

      private:
        Vec3f dir;
        Color color;

        List<ShadowMap*> shadowMaps;
        Size3Df orthoSize = { 200.f, 200.f, 1000.f };
        Mat4f matBiasVP;
    };
}