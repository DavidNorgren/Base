#pragma once

#include "util/data/vec3.hpp"
#include "util/data/vec4.hpp"
#include "util/mathfunc.hpp"


namespace Base
{
    /* 4x4 Matrix */
    template<typename T> struct Mat4
    {
        // Matrix elements
        T e[16];

        // Constructors

        Mat4() {}

        Mat4(T x1, T y1, T z1, T w1,
             T x2, T y2, T z2, T w2,
             T x3, T y3, T z3, T w3,
             T x4, T y4, T z4, T w4)
        {
            e[0] = x1; e[4] = y1; e[8] = z1; e[12] = w1;
            e[1] = x2; e[5] = y2; e[9] = z2; e[13] = w2;
            e[2] = x3; e[6] = y3; e[10] = z3; e[14] = w3;
            e[3] = x4; e[7] = y4; e[11] = z4; e[15] = w4;
        }

        inline Mat4(const Mat4& other)
        {
            for (int i = 0; i < 16; i++)
                e[i] = other[i];
        }

        // Methods

        /* Returns the transpose of the matrix.*/
        inline Mat4 transpose() const
        {
            Mat4 trans;
            for (uint i = 0; i < 4; i++)
                for (uint j = 0; j < 4; j++)
                    trans[i * 4 + j] = e[j * 4 + i];
            return trans;
        }

        /* Returns the inverse of the matrix. */
        Mat4 inverse() const
        {
            Mat4 inv = {
                 e[5] * e[10] * e[15] - e[5] * e[11] * e[14] - e[9] * e[6] * e[15] + e[9] * e[7] * e[14] + e[13] * e[6] * e[11] - e[13] * e[7] * e[10],
                -e[4] * e[10] * e[15] + e[4] * e[11] * e[14] + e[8] * e[6] * e[15] - e[8] * e[7] * e[14] - e[12] * e[6] * e[11] + e[12] * e[7] * e[10],
                 e[4] * e[9] * e[15]  - e[4] * e[11] * e[13] - e[8] * e[5] * e[15] + e[8] * e[7] * e[13] + e[12] * e[5] * e[11] - e[12] * e[7] * e[9],
                -e[4] * e[9] * e[14]  + e[4] * e[10] * e[13] + e[8] * e[5] * e[14] - e[8] * e[6] * e[13] - e[12] * e[5] * e[10] + e[12] * e[6] * e[9],

                -e[1] * e[10] * e[15] + e[1] * e[11] * e[14] + e[9] * e[2] * e[15] - e[9] * e[3] * e[14] - e[13] * e[2] * e[11] + e[13] * e[3] * e[10],
                 e[0] * e[10] * e[15] - e[0] * e[11] * e[14] - e[8] * e[2] * e[15] + e[8] * e[3] * e[14] + e[12] * e[2] * e[11] - e[12] * e[3] * e[10],
                -e[0] * e[9]  * e[15] + e[0] * e[11] * e[13] + e[8] * e[1] * e[15] - e[8] * e[3] * e[13] - e[12] * e[1] * e[11] + e[12] * e[3] * e[9],
                 e[0] * e[9]  * e[14] - e[0] * e[10] * e[13] - e[8] * e[1] * e[14] + e[8] * e[2] * e[13] + e[12] * e[1] * e[10] - e[12] * e[2] * e[9],

                 e[1] * e[6] * e[15] - e[1] * e[7] * e[14] - e[5] * e[2] * e[15] + e[5] * e[3] * e[14] + e[13] * e[2] * e[7] - e[13] * e[3] * e[6],
                -e[0] * e[6] * e[15] + e[0] * e[7] * e[14] + e[4] * e[2] * e[15] - e[4] * e[3] * e[14] - e[12] * e[2] * e[7] + e[12] * e[3] * e[6],
                 e[0] * e[5] * e[15] - e[0] * e[7] * e[13] - e[4] * e[1] * e[15] + e[4] * e[3] * e[13] + e[12] * e[1] * e[7] - e[12] * e[3] * e[5],
                -e[0] * e[5] * e[14] + e[0] * e[6] * e[13] + e[4] * e[1] * e[14] - e[4] * e[2] * e[13] - e[12] * e[1] * e[6] + e[12] * e[2] * e[5],

                -e[1] * e[6] * e[11] + e[1] * e[7] * e[10] + e[5] * e[2] * e[11] - e[5] * e[3] * e[10] - e[9] * e[2] * e[7] + e[9] * e[3] * e[6],
                 e[0] * e[6] * e[11] - e[0] * e[7] * e[10] - e[4] * e[2] * e[11] + e[4] * e[3] * e[10] + e[8] * e[2] * e[7] - e[8] * e[3] * e[6],
                -e[0] * e[5] * e[11] + e[0] * e[7] * e[9]  + e[4] * e[1] * e[11] - e[4] * e[3] * e[9]  - e[8] * e[1] * e[7] + e[8] * e[3] * e[5],
                 e[0] * e[5] * e[10] - e[0] * e[6] * e[9]  - e[4] * e[1] * e[10] + e[4] * e[2] * e[9]  + e[8] * e[1] * e[6] - e[8] * e[2] * e[5]
            };

            T det = e[0] * inv[0] + e[1] * inv[4] + e[2] * inv[8] + e[3] * inv[12];
            if (det == 0)
                return identity();

            T invDet = (T)1 / det;
            for (uint i = 0; i < 4 * 4; i++)
                inv[i] *= invDet;

            return inv;
        }

        /* Returns an identity matrix. */
        static inline Mat4 identity()
        {
            return Mat4(
                1.f, 0.f, 0.f, 0.0,
                0.f, 1.f, 0.f, 0.0,
                0.f, 0.f, 1.f, 0.0,
                0.f, 0.f, 0.f, 1.f
            );
        }

        /* Builds a translation matrix. */
        static inline Mat4 translate(const Vec3<T>& vec)
        {
            return Mat4(
                1.f, 0.f, 0.f, vec.x,
                0.f, 1.f, 0.f, vec.y,
                0.f, 0.f, 1.f, vec.z,
                0.f, 0.f, 0.f, 1.f
            );
        }

        /* Builds a rotation matrix around an axis. */
        static inline Mat4 rotate(const Vec3<T>& around, T angle)
        {
            const Vec3<T>& u = around;
            T s = dsin(angle);
            T c = dcos(angle);
            return Mat4(
                u.x * u.x + (1.f - u.x * u.x) * c, u.x * u.y * (1.f - c) - u.z * s,   u.x * u.z * (1.f - c) + u.y * s,   0.f,
                u.x * u.y * (1.f - c)  + u.z  * s, u.y * u.y + (1.f - u.y * u.y) * c, u.y * u.z * (1.f - c) - u.x * s,   0.f,
                u.x * u.z * (1.f - c)  - u.y  * s, u.y * u.z * (1.f - c) + u.x * s,   u.z * u.z + (1.f - u.z * u.z) * c, 0.f,
                0.f,                               0.f,                               0.f,                               1.f
            );
        }

        /* Builds a scaling matrix. */
        static inline Mat4 scale(const Vec3<T>& s)
        {
            return Mat4(
                s.x, 0.f, 0.f, 0.f,
                0.f, s.y, 0.f, 0.f,
                0.f, 0.f, s.z, 0.f,
                0.f, 0.f, 0.f, 1.f
            );
        }

        /* Builds an orthographic matrix. */
        static inline Mat4 ortho(T left, T right, T bottom, T top, T znear, T zfar)
        {
            return Mat4(
                2.f / (right - left), 0.f, 0.f, -(right + left) / (right - left),
                0.f, 2.f / (top - bottom), 0.f, -(top + bottom) / (top - bottom),
                0.f, 0.f, -2.f / (zfar - znear), -(zfar + znear) / (zfar - znear),
                0.f, 0.f, 0.f, 1.f
            );
        }

        /* Builds a perspective matrix. */
        static Mat4 perspective(T tFov, T ratio, T znear, T zfar)
        {
            T iFov = 1.f / tFov;
            return Mat4(
                iFov / ratio, 0.f, 0.f, 0.f,
                0.f, iFov, 0.f, 0.f,
                0.f, 0.f, (zfar + znear) / (znear - zfar), (2.f * zfar * znear) / (znear - zfar),
                0.f, 0.f, -1.f, 0.f
            );
        }

        /* Builds a view matrix from a x, y and z axis. */
        static inline Mat4 view(const Vec3<T>& xaxis, const Vec3<T>& yaxis, const Vec3<T>& zaxis)
        {
            return Mat4(
                xaxis.x, xaxis.y, xaxis.z, 0.f,
                yaxis.x, yaxis.y, yaxis.z, 0.f,
                -zaxis.x, -zaxis.y, -zaxis.z, 0.f,
                0.f, 0.f, 0.f, 1.f
            );
        }

        /* Builds a view matrix for looking at a point. */
        static inline Mat4 viewLookAt(const Vec3<T>& eye, const Vec3<T>& at, const Vec3<T>& up)
        {
            Vec3<T> look = (at - eye).normalize();
            Vec3<T> side = Vec3<T>::cross(look, up).normalize();
            Vec3<T> newUp = Vec3<T>::cross(side, look);

            return Mat4(
                side.x, side.y, side.z, 0.f,
                newUp.x, newUp.y, newUp.z, 0.f,
                -look.x, -look.y, -look.z, 0.f,
                0.f, 0.f, 0.f, 1.f
            );
        }
        
        // Get/Set via [] operator
        
        inline T operator [] (int i) const
        {
            return e[i];
        }

        inline T &operator [] (int i)
        {
            return e[i];
        }

        // Binary operators

        Mat4& operator = (const Mat4& other)
        {
            for (int i = 0; i < 16; i++)
                e[i] = other[i];
            return *this;
        }

        Mat4 operator * (const Mat4& other) const
        {
            Mat4 product;
            for (uint i = 0; i < 4; i++)
            {
                for (uint j = 0; j < 4; j++)
                {
                    product[j * 4 + i] = 0.f;

                    for (uint k = 0; k < 4; k++)
                        product[j * 4 + i] += e[k * 4 + i] * other[j * 4 + k];
                }
            }
            return product;
        }

        inline Vec3<T> operator * (const Vec3<T>& other) const
        {
            return Vec3<T>(
                e[0] * other.x + e[4] * other.y + e[8] * other.z,
                e[1] * other.x + e[5] * other.y + e[9] * other.z,
                e[2] * other.x + e[6] * other.y + e[10] * other.z
            );
        }

        inline Vec4<T> operator * (const Vec4<T>& other) const
        {
            return Vec4<T>(
                e[0] * other.x + e[4] * other.y + e[8] * other.z + e[12] * other.w,
                e[1] * other.x + e[5] * other.y + e[9] * other.z + e[13] * other.w,
                e[2] * other.x + e[6] * other.y + e[10] * other.z + e[14] * other.w,
                e[3] * other.x + e[7] * other.y + e[11] * other.z + e[15] * other.w
            );
        }
    };

    template<typename T> std::ostream& operator << (std::ostream& out, const Mat4<T>& mat)
    {
        for (uint i = 0; i < 4; i++)
        {
            for (uint j = 0; j < 4; j++)
                out << mat[j * 4 + i] << (j < 3 ? "," : "");
            out << endl;
        }
        return out;
    }

    template<typename T> inline void operator *= (Mat4<T>& a, const Mat4<T>& b)
    {
        a = a * b;
    }

    // Define shorthands
    using Mat4i = Mat4<int>;
    using Mat4f = Mat4<float>;
    using Mat4d = Mat4<double>;
}