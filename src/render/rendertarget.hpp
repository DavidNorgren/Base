#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    class RenderTarget
    {
      public:
        Size2Di getSize() const          { return size; }
        float   getRatio() const         { return (float)size.width / size.height; }
        GLuint  getGlFramebuffer() const { return glFramebuffer; }
        Mat4f   getOrtho2D() const       { return ortho2D; }
    
      protected:
        Size2Di size;
        GLuint glFramebuffer = 0;
        Mat4f ortho2D;
    };
}