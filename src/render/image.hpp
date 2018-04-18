#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "file/file.hpp"
#include "render/color.hpp"


namespace Base
{
    class Image
    {
      public:
        /* Loads an image from a file. */
        Image(string filename);
        Image(File* file);

        /* Generates an image from a solid color. */
        Image(Color color, int width, int height);

        GLuint texture;
        int width, height;

      private:
        void load(void* data);
    };
}
