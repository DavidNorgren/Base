#pragma once

#include "util/data/vec3.hpp"
#include "util/data/vec2.hpp"


namespace Base
{
    template<typename T> struct Vertex3D
    {
        Vec3<T> pos;
        Vec3f normal;
        Vec2f texCoord;
    };

    using Vertex3Di = Vertex3D<int>;
    using Vertex3Df = Vertex3D<float>;
    using Vertex3Dd = Vertex3D<double>;
}