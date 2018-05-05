#pragma once


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

        inline Size3D& operator = (const Size3D& other)
        {
            width  = other.width;
            height = other.height;
            length = other.length;
            return *this;
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Size3D<T>& size)
    {
        return out << "(" << size.width << "," << size.height << "," << size.length << ")";
    }

    // Define shorthands
    using Size3Di = Size3D<int>;
    using Size3Df = Size3D<float>;
    using Size3Dd = Size3D<double>;
}