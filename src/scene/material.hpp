#pragma once

#include "render/image.hpp"
#include "render/colors.hpp"

namespace Base
{
    class Material
    {
      public:
        Material() {}
        Material(Image* texture) : texture(texture) {}
        Image* getTexture();

      private:
        Image* texture = nullptr;
    };
}