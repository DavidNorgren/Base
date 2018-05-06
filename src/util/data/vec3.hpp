#pragma once

#include "util/mathfunc.hpp"


namespace Base
{
    /* 3D vector */
    template<typename T> struct Vec3 
    {
        // Vector elements
        T x, y, z;

        // Constructors

        Vec3() {}
        
        Vec3(T x)
        {
            this->x = y = z = x;
        }

        Vec3(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vec3(T x[3])
        {
            this->x = x[0];
            y = x[1];
            z = x[2];
        }

        Vec3(const Vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }

        // Methods

        inline T length() const
        {
            return sqrt(x * x + y * y + z * z);
        }

        inline Vec3 normalize() const
        {
            T len = length();
            if (len == 0)
                return Vec3((T)0);
            return Vec3(x / len, y / len, z / len);
        }

        inline Vec3 rotate(const Vec3&a, float angle) const
        {
            float d = dot(this, a);
            float c = dcos(angle);
            float ci = 1.f - c;
            float s = dsin(angle);
            return Vec3(
                a.x * d * ci + x * c + (-a.z * y + a.x * z) * s,
                a.y * d * ci + y * c + (a.z * x - a.y * z) * s,
                a.z * d * ci + z * c + (-a.x * x + a.y * y) * s
            );
        }

        static inline T distance(const Vec3& a, const Vec3& b)
        {
            return length(Vec3(a.x - b.x, a.y - b.y, a.z - b.z));
        }

        static inline T dot(const Vec3& a, const Vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }
        
        static inline bool approxEq(const Vec3& a, const Vec3& b)
        {
            return (approxEq(a.x, b.x) && approxEq(a.y, b.y) && approxEq(a.z, b.z));
        }

        static inline Vec3 cross(const Vec3& a, const Vec3& b)
        {
            return Vec3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }
        
        // Get/Set via [] operator
        
        inline T operator [] (int i) const
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            return z;
        }

        inline T& operator [] (int i)
        {
            if (i == 0)
                return x;
            else if (i == 1)
                return y;
            return z;
        }

        // Unary operators

        inline Vec3 operator - () const
        {
            return Vec3(-x, -y, -z);

        }
        
        // Binary operators

        inline Vec3& operator = (const Vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        inline Vec3 operator + (const Vec3& other) const
        {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }

        inline void operator += (const Vec3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        }

        inline Vec3 operator - (const Vec3& other) const
        {
            return Vec3(x - other.x, y - other.y, z - other.z); 
        }

        inline void operator -= (const Vec3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }

        inline Vec3 operator * (const T& mul) const
        {
            return Vec3(x * mul, y * mul, z * mul);
        }

        inline void operator *= (const T& mul)
        {
            x *= mul;
            y *= mul;
            z *= mul;
        }

        inline void operator *= (const Vec3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
        }

        inline Vec3 operator / (const T& mul) const
        {
            return Vec3(x / mul, y / mul, z / mul);
        }

        inline void operator /= (const T& mul)
        {
            x /= mul;
            y /= mul;
            z /= mul;
        }

        // Comparison operators

        inline bool operator == (const Vec3& other) const
        {
            return (x == other.x && y == other.y && z == other.z);
        }

        inline bool operator != (const Vec3& other) const
        {
            return !(this == other);
        }
    };

    // Convert to string

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Vec3<T>& vec)
    {
        return out << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    }

    // Multiply by factor (reversed)

    template<typename T> inline Vec3<T> operator * (const T& mul, const Vec3<T>& vec)
    {
        return vec * mul;
    }

    // Define shorthands
    
    using Vec3i  = Vec3<int>;
    using Vec3ui = Vec3<uint>;
    using Vec3f  = Vec3<float>;
    using Vec3d  = Vec3<double>;
}