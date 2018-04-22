#pragma once

#include <math.h>


namespace Base
{
    /* 3D vector */
    template<typename T> struct Vec3 
    {
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
            this->y = x[1];
            this->z = x[2];
        }

        inline Vec3(const Vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }

        // Methods

        inline T length()
        {
            return sqrt(x * x + y * y + z * z);
        }

        inline Vec3 normalize()
        {
            T len = length();
            if (len == (T)0)
                return Vec3((T)0);
            return Vec3(x / len, y / len, z / len);
        }

        static inline T distance(const Vec3& a, const Vec3& b)
        {
            return length(Vec3(a.x - b.x, a.y - b.y, a.z - b.z));
        }

        static inline T dot(const Vec3& a, const Vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static inline Vec3 cross(const Vec3& a, const Vec3& b)
        {
            return Vec3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        /* TODO
        static inline Vec3 reflect(const Vec3& incidence, const Vec3& normal)
        {
            return embree::reflect(incidence.eVec, normal.eVec);
        }

        static inline Vec3 refract(const Vec3& incidence, const Vec3& normal, const T& ior)
        {
            Vec3 nn = normal;
            T negNdotV = dot(incidence, nn), eta;

            if (negNdotV > 0.0f) {
                eta = ior;
                nn = -normal.eVec;
                negNdotV = -negNdotV;
            } else {
                eta = 1.f / ior;
            }

            const T k = 1.f - eta*eta * (1.f - negNdotV * negNdotV);

            if (k < 0.0f) {
                return incidence;
            } else {
                return normalize(eta * incidence.eVec - (eta*negNdotV + sqrtf(k)) * nn.eVec);
            }

        }

        static inline Vec3 rotate(const Vec3& vec, const Vec3& around, T angle)
        {
            return embree::xfmVector(embree::AffineSpace3fa::rotate(around.eVec, embree::deg2rad(angle)), vec.eVec);
        }*/

        // Unary operators

        inline Vec3 operator - ()
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


        inline Vec3<T> operator - (const Vec3<T>& other) const
        {
            return Vec3<T>(x - other.x, y - other.y, z - other.z); 
        }

        inline void operator -= (const Vec3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }

        inline Vec3 operator * (const T& mul) const
        {
            return Vec3(mul * x, mul * y, mul * z);
        }

        inline void operator *= (const T& mul)
        {
            x *= mul;
            y *= mul;
            z *= mul;
        }

        // Comparison operators

        inline bool operator == (const Vec3& other)
        {
            return (x == other.x && y == other.y && z == other.z);
        }

        inline bool operator != (const Vec3& other)
        {
            return !(this == other);
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& cout, const Vec3<T>& vec)
    {
        return cout << "(" << vec.x << "," << vec.y << "," << vec.z << ")" << endl;
    }

    template<typename T> inline Vec3<T> operator * (const T& mul, const Vec3<T>& vec)
    {
        return Vec3<T>(mul * vec.x, mul * vec.y, mul * vec.z);
    }

    // Define shorthands
    using Vec3i  = Vec3<int>;
    using Vec3ui = Vec3<uint>;
    using Vec3f  = Vec3<float>;
    using Vec3d  = Vec3<double>;
}