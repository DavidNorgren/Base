#pragma once

#include "render/image.hpp"
#include "render/color.hpp"
#include "resource/resource.hpp"
#include "util/data/size2d.hpp"


namespace Base
{
    /* A static image that is used as a graphic in the scene. */
    class Sprite : public Image, public Resource
    {
      public:
        Sprite() {};

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(uchar* pixelData);
        void cleanUp() override;
    };
}
