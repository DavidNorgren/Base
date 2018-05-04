#pragma once


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

        inline Size2D& operator = (const Size2D& other)
        {
            width  = other.width;
            height = other.height;
            return *this;
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Size2D<T>& size)
    {
        return out << "(" << size.width << "," << size.height << ")";
    }

    // Define shorthands
    using Size2Di = Size2D<int>;
    using Size2Df = Size2D<float>;
    using Size2Dd = Size2D<double>;
}