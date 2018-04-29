#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "resource/resource.hpp"
#include "render/color.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    class Image : public Resource
    {
      public:
        Image() {};
        
        /* Generates an image from a solid color. */
        EXPORT Image(const Color& color);

        EXPORT GLuint getGlTexture() { return glTexture; };
        EXPORT Size2Di getSize()     { return size; };

      private:
        GLuint glTexture;
        Size2Di size;

        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(uchar* pixelData);
        void cleanUp() override;
    };
}
