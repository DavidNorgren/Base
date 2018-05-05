#pragma once

#include "util/mathfunc.hpp"
#include "util/data/vec3.hpp"


namespace Base
{
    /* 3D homogenous vector */
    template<typename T> struct Vec4 
    {
        T x, y, z, w;

        // Constructors

        Vec4() {}
        
        Vec4(T x)
        {
            this->x = y = z = x;
            w = 1;
        }

        Vec4(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            w = 1;
        }

        Vec4(T x, T y, T z, T w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        Vec4(T x[4])
        {
            this->x = x[0];
            y = x[1];
            z = x[2];
            w = x[3];
        }

        inline Vec4(const Vec3<T>& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = 1;
        }
        
        inline Vec4(const Vec4& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
        }

        // Methods

        inline T length() const
        {
            return sqrt(x * x + y * y + z * z + w * w);
        }

        inline Vec4 normalize() const
        {
            T len = length();
            if (len == 0)
                return Vec4(0);
            return Vec4(x / len, y / len, z / len, w / len);
        }

        inline Vec3<T> homogenize() const
        {
            return Vec3<T>(x / w, y / w, z / w);
        }

        static inline T dot(const Vec4& a, const Vec4& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }
        
        static inline bool approxEq(const Vec4& a, const Vec4& b)
        {
            return (approxEq(a.x, b.x) &&
                    approxEq(a.y, b.y) &&
                    approxEq(a.z, b.z) &&
                    approxEq(a.w, b.w));
        }
        
        // Get/Set via [] operator
        
        inline T operator [] (int i) const
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            else if (i == 2)
                return z;
            return w;
        }

        inline T& operator [] (int i)
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            else if (i == 2)
                return z;
            return w;
        }

        // Unary operators

        inline Vec4 operator - () const
        {
            return Vec4(-x, -y, -z, -w);

        }
        
        // Binary operators

        inline Vec4& operator = (const Vec4& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        inline Vec4 operator + (const Vec4& other) const
        {
            return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        inline void operator += (const Vec4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
        }


        inline Vec4 operator - (const Vec4& other) const
        {
            return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); 
        }

        inline void operator -= (const Vec4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
        }

        inline Vec4 operator * (const T& mul) const
        {
            return Vec4(x * mul, y * mul, z * mul, w * mul);
        }

        inline void operator *= (const T& mul)
        {
            x *= mul;
            y *= mul;
            z *= mul;
            w *= mul;
        }

        inline void operator *= (const Vec4& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
        }

        inline Vec4 operator / (const T& mul) const
        {
            return Vec4(x / mul, y / mul, z / mul, w / mul);
        }

        inline void operator /= (const T& mul)
        {
            x /= mul;
            y /= mul;
            z /= mul;
            w /= mul;
        }

        // Comparison operators

        inline bool operator == (const Vec4& other) const
        {
            return (x == other.x && y == other.y && z == other.z && w == other.w);
        }

        inline bool operator != (const Vec4& other) const
        {
            return !(this == other);
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Vec4<T>& vec)
    {
        return out << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
    }

    template<typename T> inline Vec4<T> operator * (const T& mul, const Vec4<T>& vec)
    {
        return Vec4<T>(mul * vec.x, mul * vec.y, mul * vec.z, mul * vec.w);
    }

    // Define shorthands
    using Vec4i  = Vec4<int>;
    using Vec4ui = Vec4<uint>;
    using Vec4f  = Vec4<float>;
    using Vec4d  = Vec4<double>;
}