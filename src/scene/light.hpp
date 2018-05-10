#pragma once

#include "scene/camera.hpp"
#include "scene/scene.hpp"
#include "render/color.hpp"
#include "render/rendertarget.hpp"
#include "render/surface.hpp"
#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"
#include "scene/model.hpp"


namespace Base
{
    constexpr float LIGHT_CASCADES[] = { 0.f, 0.25f, 0.5f, 1.f };

    /* The format of the shadow mapping for this light. */
    enum class ShadowMapFormat
    {
        SINGLE,
        CASCADE,
        CUBE
    };

    /* A light in the scene. */
    class Scene;
    class ShadowMap;
    class Light
    {
      public:
        EXPORT Light();
        EXPORT ~Light();

        /* Color */
        void setColor(const Color& color) { this->color = color; }
        const Color& getColor() const     { return color; }
        
        /* Position in the scene. */
        EXPORT void setPosition(const Vec3f& position);
        EXPORT Light* translate(const Vec3f& translate);
        const Vec3f& getPosition() const { return pos; };
        const Vec3f& getDir() const      { return dir; }

        /* Prepares a pass for shading mapping. */
        EXPORT void prepareShadowMaps(const Scene* scene);

        /* Returns references to the generated shadowmaps. */
        const List<ShadowMap*>& getShadowMaps() const { return shadowMaps; }

      private:
        Vec3f pos, dir;
        Color color;
        List<ShadowMap*> shadowMaps;
    };

    /* A depth map for shadows. */
    class ShadowMap : public RenderTarget, public Camera
    {
      public:
        EXPORT ShadowMap(Size2Di size);
        const Mat4f& getBiasViewProjection() const { return matBiasVP; };
        float getCascadeEndClipSpaceDepth() const  { return cascadeEndClipSpaceDepth; }
        Surface* getBlurSurface() const { return blurSurface; }

        Model* debugCamFrustum;
        Model* debugOrthoBox;

      friend class Light;
      protected:
        Mat4f matBiasVP;
        float cascadeEndClipSpaceDepth;
        Material* debugMaterial;
        TriangleMesh* debugCamFrustumMesh;
        TriangleMesh* debugOrthoBoxMesh;
        Surface* blurSurface;
    };
}