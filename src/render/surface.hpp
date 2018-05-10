#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "render/image.hpp"
#include "render/rendertarget.hpp"


namespace Base
{
    /* A type of image that can be rendered to and modified during runtime. */
    class Surface : public RenderTarget
    {
      public:
        EXPORT Surface();
        EXPORT Surface(Size2Di size, GLint internalFormat = GL_RGBA, GLint format = GL_RG);

        EXPORT void resize(Size2Di size);
        EXPORT void clear();

      private:
        GLint internalFormat = GL_RGBA;
        GLint format = GL_RG;
    };
}