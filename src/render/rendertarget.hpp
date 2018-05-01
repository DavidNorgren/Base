#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "render/image.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    /* A 2 dimensional image that can be rendered to. */
    class RenderTarget : public Image
    {
      public:
        GLuint getGlFramebuffer() const { return glFramebuffer; }
        Mat4f  getOrtho2D() const       { return ortho2D; }
    
      protected:
        GLuint glFramebuffer = 0;
        GLuint glDepthRenderbuffer = 0;
        Mat4f  ortho2D;
    };
}