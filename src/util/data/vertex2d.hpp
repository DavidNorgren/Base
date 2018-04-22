#pragma once

#include "util/data/vec2.hpp"


namespace Base
{
    template<typename T> struct Vertex2D
    {
        Vec2<T> pos;
        Vec2f texCoord;
    };

    using Vertex2Di = Vertex2D<int>;
    using Vertex2Df = Vertex2D<float>;
    using Vertex2Dd = Vertex2D<double>;
}