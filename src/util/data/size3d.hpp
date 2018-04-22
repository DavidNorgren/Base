#pragma once


namespace Base
{
    template<typename T> struct Size3D
    {
        T width, length, height;
        
        Size3D() {}

        Size3D(T width, T length, T height)
        {
            this.width = width;
            this.length = length;
            this.height = height;
        }
        
        inline Size3D(const Size3D& other)
        {
            width = other.width;
            length = other.length;
            height = other.height;
        }

        inline Size3D& operator = (const Size3D& other)
        {
            width = other.width;
            length = other.length;
            height = other.height;
            return *this;
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& cout, const Size3D<T>& size)
    {
        return cout << "(" << size.width << "," << size.length << "," << size.height << ")" << endl;
    }

    // Define shorthands
    using Size3Di = Size3D<int>;
    using Size3Df = Size3D<float>;
    using Size3Dd = Size3D<double>;
}