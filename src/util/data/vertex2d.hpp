#pragma once

#include "util/data/tex2.hpp"
#include "util/data/vec2.hpp"


namespace Base
{
    /* A vertex in 2D space, containing a position and texture coordinate. */
    template<typename T> struct Vertex2D
    {
        // Vertex elements
        Vec2<T> pos;
        Tex2f texCoord;

        Vertex2Df() {}

        // Comparison operators

        inline bool operator == (const Vertex2D& other)
        {
            return (pos == other.pos && texCoord == other.texCoord);
        }

        inline bool operator != (const Vertex2D& other)
        {
            return !(this == other);
        }
    };

    // Define shorthands

    using Vertex2Di = Vertex2D<int>;
    using Vertex2Df = Vertex2D<float>;
    using Vertex2Dd = Vertex2D<double>;
}