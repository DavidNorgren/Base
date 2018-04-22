#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "file/file.hpp"
#include "render/color.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    class Image
    {
      public:
        /* Loads an image from a file. */
        Image(string filename);
        Image(File* file);

        /* Generates an image from a solid color. */
        Image(Color color, Size2Di size);

        GLuint glTexture;
        Size2Di glTextureSize;

      private:
        void load(void* data);
    };
}
