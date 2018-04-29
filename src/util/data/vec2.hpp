#pragma once

#include "util/mathfunc.hpp"


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

        inline T length()
        {
            return sqrt(x * x + y * y);
        }

        inline Vec2 normalize()
        {
            T len = length();
            if (len == (T)0)
                return Vec2((T)0);
            return Vec2(x / len, y / len);
        }

        static inline T distance(const Vec2& a, const Vec2& b)
        {
            return length(Vec2(a.x - b.x, a.y - b.y));
        }

        static inline T dot(const Vec2& a, const Vec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }
        
        static inline bool approxEq(const Vec2& a, const Vec2& b)
        {
            return (approxEq(a.x, b.x) && approxEq(a.y, b.y));
        }

        // Unary operators

        inline T operator [] (int i) const
        {
            if (i == 0)
                return x;
            return y;
        }

        inline T& operator [] (int i)
        {
            if (i == 0)
                return x;
            return y;
        }
        
        // Binary operators

        inline Vec2& operator = (const Vec2& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        inline Vec2 operator + (const Vec2& other) const
        {
            return Vec2(x + other.x, y + other.y);
        }

        inline void operator += (const Vec2& other)
        {
            x += other.x;
            y += other.y;
        }

        inline Vec2 operator - (const Vec2& other) const
        {
            return Vec2(x - other.x, y - other.y); 
        }

        inline void operator -= (const Vec2& other)
        {
            x -= other.x;
            y -= other.y;
        }

        inline Vec2 operator * (const T& mul) const
        {
            return Vec2(mul * x, mul * y);
        }

        inline void operator *= (const T& mul)
        {
            x *= mul;
            y *= mul;
        }

        inline void operator *= (const Vec2& other)
        {
            x *= other.x;
            y *= other.y;
        }

        // Comparison operators

        inline bool operator == (const Vec2& other) const
        {
            return (x == other.x && y == other.y);
        }

        inline bool operator != (const Vec2& other) const
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