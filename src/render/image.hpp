#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "render/color.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    /* A general 2 dimensional texture that can be rendered. */
    class Image
    {
      public:
        EXPORT Image() {}
        EXPORT ~Image();

        /* Get the dimensions of the image. */
        EXPORT Size2Di getSize() const     { return size; };
        EXPORT float   getRatio() const    { return (float)size.width / size.height; }

        /* Get OpenGL texture used for rendering. */
        EXPORT GLuint  getGlTexture() const { return glTexture; };
        
        /* Generates an image from a solid color. */
        EXPORT static Image* createSingleColor(const Color& color);

      protected:
        Size2Di size;
        GLuint glTexture = 0;
    };
}