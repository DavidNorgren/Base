#pragma once

#include "util/data/vec2.hpp"


namespace Base
{
    template<typename T> struct Size2D
    {
        union
        {
            struct { T width, height; };
            struct { T x, y; };
        };
        
        Size2D() {}

        Size2D(T width, T height)
        {
            this->width  = width;
            this->height = height;
        }
        
        inline Size2D(const Size2D& other)
        {
            width  = other.width;
            height = other.height;
        }
        
        inline Size2D(const Vec2<T>& other)
        {
            width  = other.x;
            height = other.y;
        }

        inline Size2D& operator = (const Size2D& other)
        {
            width  = other.width;
            height = other.height;
            return *this;
        }

        inline Size2D operator * (const T& mul) const
        {
            return Size2D(x * mul, y * mul);
        }

        inline Size2D operator / (const T& mul) const
        {
            return Size2D(x / mul, y / mul);
        }

        // Comparison operators

        inline bool operator == (const Size2D& other) const
        {
            return (x == other.x && y == other.y);
        }

        inline bool operator != (const Size2D& other) const
        {
            return !(this == other);
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Size2D<T>& size)
    {
        return out << "(" << size.width << "," << size.height << ")";
    }

    template<typename T> inline Size2D<T> operator * (const T& mul, const Size2D<T>& size)
    {
        return Size2D<T>(mul * size.x, mul * size.y);
    }

    // Define shorthands
    using Size2Di = Size2D<int>;
    using Size2Df = Size2D<float>;
    using Size2Dd = Size2D<double>;
}