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

        GLuint glTexture;
        Size2Di glTextureSize;

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(uchar* pixelData);
        bool reload(const FilePath& file) override;
    };
}
