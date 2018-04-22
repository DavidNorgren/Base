#pragma once

#include <math.h>


namespace Base
{
    /* 2D vector */
    template<typename T> struct Vec2
    {
        T x, y;

        // Constructors

        Vec2() {}

        Vec2(T x)
        {
            this->x = y = x;
        }

        Vec2(T x, T y)
        {
            this->x = x;
            this->y = y;
        }

        Vec2(T x[2])
        {
            this->x = x[0];
            this->y = x[1];
        }

        inline Vec2(const Vec2& other)
        {
            x = other.x;
            y = other.y;
        }

        // Methods

        static inline T length(const Vec2& a)
        {
            return sqrt(a.x * a.x + a.y * a.y);
        }

        static inline Vec2 normalize(const Vec2& a)
        {
            T len = length(a);
            if (len == (T)0)
                return Vec2((T)0);
            return Vec2(a.x / len, a.y / len);
        }

        static inline T distance(const Vec2& a, const Vec2& b)
        {
            return length(Vec2(a.x - b.x, a.y - b.y));
        }

        static inline T dot(const Vec2& a, const Vec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }

        // Unary operators

        inline Vec2 operator - ()
        {
            return Vec2(-x, -y);

        }
        
        // Binary operators

        inline Vec2& operator = (const Vec2& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        inline Vec2 operator + (const Vec2& other)
        {
            return Vec2(x + other.x, y + other.y);
        }

        inline void operator += (const Vec2& other)
        {
            x += other.x;
            y += other.y;
        }

        inline Vec2 operator - (const Vec2& other)
        {
            return Vec2(other.x, other.y); 
        }

        inline void operator -= (const Vec2& other)
        {
            x -= other.x;
            y -= other.y;
        }

        inline Vec2 operator * (const T& mul)
        {
            return Vec2(mul * x, mul * y);
        }

        inline void operator *= (const T& mul)
        {
            x *= mul;
            y *= mul;
        }

        // Comparison operators

        inline bool operator == (const Vec2& other)
        {
            return (x == other.x && y == other.y);
        }

        inline bool operator != (const Vec2& other)
        {
            return !(this == other);
        }
    };

    template<typename T> inline std::ostream& operator << (std::ostream& cout, const Vec2<T>& vec)
    {
        return cout << "(" << vec.x << "," << vec.y << ")" << endl;
    }

    template<typename T> inline Vec2<T> operator * (const T& mul, const Vec2<T>& vec)
    {
        return Vec2<T>(mul * vec.x, mul * vec.y);
    }

    // Define shorthands
    using Vec2i     = Vec2<int>;
    using Vec2ui    = Vec2<uint>;
    using Vec2f     = Vec2<float>;
    using Vec2d     = Vec2<double>;
    using ScreenPos = Vec2<int>;
}