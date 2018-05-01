#pragma once

#include "scene/camera.hpp"
#include "render/color.hpp"
#include "render/rendertarget.hpp"
#include "util/data/vec3.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"

namespace Base
{
    class Light : public Camera, public RenderTarget
    {
      public:
        Light();
        ~Light();

        void setColor(const Color& color) { this->color = color; }
        const Color& getColor() const { return color; }
        
        void setPosition(const Vec3f& position);
        Light* translate(const Vec3f& translate);
        void buildMatrix(float ratio) override;

        const Vec3f& getDir() const                { return dir; }
        GLuint getGlDepthTexture() const           { return glDepthTexture; }
        const Mat4f& getBiasViewProjection() const { return matBiasPV; }

      private:
        Vec3f dir;
        Color color;

        GLuint glDepthTexture;
        Size3Df orthoSize = { 200.f, 200.f, 1000.f };
        Mat4f matBiasPV;
    };
}