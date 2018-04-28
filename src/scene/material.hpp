#pragma once

#include "resource/image.hpp"
#include "render/colors.hpp"


namespace Base
{
    class Material
    {
      public:
        Material() { texture = new Image(Colors::GREEN); }
        Material(Image* texture) : texture(texture) {}
        Image* texture;
    };
}