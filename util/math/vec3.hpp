#pragma once

#include <math.h>
#include <iostream>


namespace Base
{

    /* 3D vector */
    struct Vec3 
    {
        float x, y, z;

        //// Constructors ////

        Vec3()
        {
            x = y = z = 0.f;
        }


        Vec3(float x)
        {
            this->x = y = z = x;
        }


        Vec3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }


        Vec3(int x, int y, int z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }


        Vec3(float x[3])
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


        inline Vec3& operator=(const Vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }


        //// Functions ////

        static inline float length(const Vec3& a)
        {
            return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        }


        static inline Vec3 normalize(const Vec3& a)
        {
            float len = length(a);
            if (len == 0.f) {
                return Vec3(0.f);
            }
            return Vec3(a.x / len, a.y / len, a.z / len);
        }


        static inline float distance(const Vec3& a, const Vec3& b)
        {
            return length(Vec3(a.x - b.x, a.y - b.y, a.z - b.z));
        }


        static inline float dot(const Vec3& a, const Vec3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }


        /* TODO
        static inline Vec3 cross(const Vec3& a, const Vec3& b)
        {
            return embree::cross(a.eVec, b.eVec);
        }


        static inline Vec3 reflect(const Vec3& incidence, const Vec3& normal)
        {
            return embree::reflect(incidence.eVec, normal.eVec);
        }


        static inline Vec3 refract(const Vec3& incidence, const Vec3& normal, const float& ior)
        {
            Vec3 nn = normal;
            float negNdotV = dot(incidence, nn), eta;

            if (negNdotV > 0.0f) {
                eta = ior;
                nn = -normal.eVec;
                negNdotV = -negNdotV;
            } else {
                eta = 1.f / ior;
            }

            const float k = 1.f - eta*eta * (1.f - negNdotV * negNdotV);

            if (k < 0.0f) {
                return incidence;
            } else {
                return normalize(eta * incidence.eVec - (eta*negNdotV + sqrtf(k)) * nn.eVec);
            }

        }


        static inline Vec3 rotate(const Vec3& vec, const Vec3& around, float angle)
        {
            return embree::xfmVector(embree::AffineSpace3fa::rotate(around.eVec, embree::deg2rad(angle)), vec.eVec);
        }*/

    };


    //// Unary operators ////

    inline Vec3 operator - (const Vec3& a)
    {
        return Vec3(-a.x, -a.y, -a.z);
    }


    inline std::ostream& operator << (std::ostream& cout, const Vec3& a)
    {
        return cout << "(" << a.x << "," << a.y << "," << a.z << ")" << std::endl;
    }


    //// Binary operators ////

    inline Vec3 operator + (const Vec3& a, const Vec3& b)
    {
        return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
    }


    inline void operator += (Vec3& a, const Vec3& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
    }


    inline Vec3 operator - (const Vec3& a, const Vec3& b)
    {
        return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); 
    }


    inline void operator -= (Vec3& a, const Vec3& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
    }


    inline Vec3 operator * (const float& a, const Vec3& b)
    {
        return Vec3(a * b.x, a * b.y, a * b.z);
    }


    inline Vec3 operator * (const Vec3& a, const float& b)
    {
        return Vec3(b * a.x, b * a.y, b * a.z);
    }


    inline void operator *= (Vec3& a, const float& b)
    {
        a.x *= b;
        a.y *= b;
        a.z *= b;
    }


    //// Comparison operators ////

    inline bool operator == (const Vec3& a, const Vec3& b)
    {
        return (a.x == b.x && a.y == b.y && a.z == b.z);
    }


    inline bool operator != (const Vec3& a, const Vec3& b)
    {
        return !(a == b);
    }

}