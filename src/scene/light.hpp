#pragma once

#include "render/color.hpp"
#include "util/data/vec3.hpp"

namespace Base
{
    class Light
    {
      public:
        Light() {}

        Light* translate(const Vec3f& translate);

        Vec3f pos;
        Vec3f dir;
        Color color;
    };
}