#pragma once

#include "util/mathfunc.hpp"


namespace Base
{
    /* 2D vector */
    template<typename T> struct Vec2
    {
        // Vector elements
        union
        {
            struct { T x, y; };
            struct { T u, v; };
        };

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
            y = x[1];
        }

        Vec2(const Vec2& other)
        {
            x = other.x;
            y = other.y;
        }

        // Methods

        inline T length() const
        {
            return sqrt(x * x + y * y);
        }

        inline Vec2 normalize() const
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

        static inline Vec2 min(const Vec2& a, const Vec2& b)
        {
            return Vec2(Base::min(a.x, b.x), Base::min(a.y, b.y));
        }

        static inline Vec2 max(const Vec2& a, const Vec2& b)
        {
            return Vec2(Base::max(a.x, b.x), Base::max(a.y, b.y));
        }

        inline Vec2 floor() const
        {
            return Vec2(Base::floor(x), Base::floor(y));
        }

        inline Vec2 ceil() const
        {
            return Vec2(Base::ceil(x), Base::ceil(y));
        }

        inline Vec2 round() const
        {
            return Vec2(Base::round(x), Base::round(y));
        }
        
        // Get/Set via [] operator

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

        // Unary operators

        inline Vec2 operator - () const
        {
            return Vec2(-x, -y);
        }
        
        // Binary operators

        Vec2& operator = (const Vec2& other)
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

        inline Vec2 operator * (const Vec2& other) const
        {
            return Vec2(x * other.x, y * other.y); 
        }

        inline void operator *= (const Vec2& other)
        {
            x *= other.x;
            y *= other.y;
        }

        template<typename R> inline Vec2 operator * (const R& mul) const
        {
            return Vec2(x * mul, y * mul);
        }

        template<typename R> inline void operator *= (const R& mul)
        {
            x *= mul;
            y *= mul;
        }

        inline Vec2 operator / (const Vec2& other) const
        {
            return Vec2(x / other.x, y / other.y); 
        }

        inline void operator /= (const Vec2& other)
        {
            x /= other.x;
            y /= other.y;
        }

        template<typename R> inline Vec2 operator / (const R& mul) const
        {
            return Vec2(x / mul, y / mul);
        }

        template<typename R> inline void operator /= (const R& mul)
        {
            x /= mul;
            y /= mul;
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

    // Convert to string

    template<typename T> inline std::ostream& operator << (std::ostream& out, const Vec2<T>& vec)
    {
        return out << "(" << vec.x << "," << vec.y << ")";
    }

    // Multiply by factor (reversed)

    template<typename T> inline Vec2<T> operator * (const T& mul, const Vec2<T>& vec)
    {
        return vec * mul;
    }

    // Define shorthands
    
    using Vec2i     = Vec2<int>;
    using Vec2ui    = Vec2<uint>;
    using Vec2f     = Vec2<float>;
    using Vec2d     = Vec2<double>;
    using ScreenPos = Vec2<int>;
}