#pragma once

#include "util/data/vec3.hpp"


namespace Base
{
    template<typename T> struct Size3D
    {
        // Size elements
        union
        {
            struct { T width, height, length; };
            struct { T x, y, z; };
        };

        // Constructors

        Size3D() {}

        Size3D(T width, T height, T length)
        {
            this->width  = width;
            this->height = height;
            this->length = length;
        }
        
        Size3D(const Size3D& other)
        {
            width  = other.width;
            height = other.height;
            length = other.length;
        }
        
        Size3D(const Vec3<T>& other)
        {
            width  = other.x;
            height = other.y;
            length = other.z;
        }

        // Binary operators

        Size3D& operator = (const Size3D& other)
        {
            width  = other.width;
            height = other.height;
            length = other.length;
            return *this;
        }

        template<typename R> inline Size3D operator * (const R& mul) const
        {
            return Size3D(x * mul, y * mul, z * mul);
        }

        template<typename R> inline Size3D operator / (const R& mul) const
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

        // Implicit cast to vector

        operator Vec3<T>() const { return Vec3<T>(width, height, length); }
    };
    
    // Convert to string

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Size3D<T>& size)
    {
        return out << "(" << size.width << "," << size.height << "," << size.length << ")";
    }

    // Multiply by factor (reversed)

    template<typename T, typename R> inline Size3D<T> operator * (const T& mul, const Size3D<R>& size)
    {
        return size * mul;
    }

    // Define shorthands
    
    using Size3Di = Size3D<int>;
    using Size3Df = Size3D<float>;
    using Size3Dd = Size3D<double>;
}