#pragma once

#include "util/data/vec3.hpp"


namespace Base
{
    template<typename T> struct Size3D
    {
        union
        {
            struct { T width, height, length; };
            struct { T x, y, z; };
        };

        Size3D() {}

        Size3D(T width, T height, T length)
        {
            this->width  = width;
            this->height = height;
            this->length = length;
        }
        
        inline Size3D(const Size3D& other)
        {
            width  = other.width;
            height = other.height;
            length = other.length;
        }
        
        inline Size3D(const Vec3<T>& other)
        {
            width  = other.x;
            height = other.y;
            length = other.z;
        }

        inline Size3D& operator = (const Size3D& other)
        {
            width  = other.width;
            height = other.height;
            length = other.length;
            return *this;
        }

        inline Size3D operator * (const T& mul) const
        {
            return Size3D(x * mul, y * mul, z * mul);
        }

        inline Size3D operator / (const T& mul) const
        {
            return Size3D(x / mul, y / mul, z / mul);
        }

        // Comparison operators

        inline bool operator == (const Size3D& other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }

        inline bool operator != (const Size3D& other) const
        {
            return !(this == other);
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Size3D<T>& size)
    {
        return out << "(" << size.width << "," << size.height << "," << size.length << ")";
    }

    template<typename T> inline Size3D<T> operator * (const T& mul, const Size3D<T>& size)
    {
        return Size3D<T>(mul * size.x, mul * size.y, mul * size.z);
    }

    // Define shorthands
    using Size3Di = Size3D<int>;
    using Size3Df = Size3D<float>;
    using Size3Dd = Size3D<double>;
}