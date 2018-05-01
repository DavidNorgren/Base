#pragma once

#include "util/data/tex2.hpp"
#include "util/data/vec2.hpp"
#include "util/data/vec3.hpp"


namespace Base
{
    template<typename T> struct Vertex3D
    {
        Vec3<T> pos;
        Tex2f texCoord;
        Vec3f normal;

        Vertex3D() {}
        
        Vertex3D(const Vec3<T>& pos)
        {
            this->pos = pos;
            this->texCoord = Tex2f(0.f);
            this->normal = Vec3f(0.f);
        }

        Vertex3D(const Vec3<T>& pos, const Tex2f texCoord, const Vec3f& normal)
        {
            this->pos = pos;
            this->texCoord = texCoord;
            this->normal = normal;
        }

        // Comparison operators

        inline bool operator == (const Vertex3D& other) const
        {
            return (pos == other.pos && texCoord == other.texCoord && normal == other.normal);
        }

        inline bool operator != (const Vertex3D& other) const
        {
            return !(this == other);
        }
    };

    using Vertex3Di = Vertex3D<int>;
    using Vertex3Df = Vertex3D<float>;
    using Vertex3Dd = Vertex3D<double>;
}