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
        Size2Di getSize() const  { return size; };
        float   getRatio() const { return (float)size.width / size.height; }

        /* Get OpenGL texture used in shaders. */
        GLuint  getGlTexture() const { return glTexture; };
        
        /* Returns an image of a solid color. */
        EXPORT static Image* getSingleColor(const Color& color);

      protected:
        Size2Di size = { 0, 0 };
        GLuint glTexture = 0;
    };
}