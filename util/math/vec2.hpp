#pragma once

#include <math.h>
#include <iostream>


namespace Base
{
    
    /* 2D vector */
    struct Vec2
    {
        float x, y;

        //// Constructors ////

        Vec2()
        {
            x = y = 0.f;
        }


        Vec2(float x)
        {
            this->x = y = x;
        }


        Vec2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }


        Vec2(int x, int y)
        {
            this->x = x;
            this->y = y;
        }


        Vec2(float x[2])
        {
            this->x = x[0];
            this->y = x[1];
        }


        inline Vec2(const Vec2& other)
        {
            x = other.x;
            y = other.y;
        }


        inline Vec2& operator=(const Vec2& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }


        //// Functions ////

        static inline float length(const Vec2& a)
        {
            return sqrt(a.x * a.x + a.y * a.y);
        }


        static inline Vec2 normalize(const Vec2& a)
        {
            float len = length(a);
            if (len == 0.f)
                return Vec2(0.f);
            return Vec2(a.x / len, a.y / len);
        }


        static inline float distance(const Vec2& a, const Vec2& b)
        {
            return length(Vec2(a.x - b.x, a.y - b.y));
        }


        static inline float dot(const Vec2& a, const Vec2& b)
        {
            return a.x * b.x + a.y * b.y;
        }
    };


    //// Unary operators ////

    inline Vec2 operator - (const Vec2& a)
    {
        return Vec2(-a.x, -a.y);
    }


    inline std::ostream& operator << (std::ostream& cout, const Vec2& a)
    {
        return cout << "(" << a.x << "," << a.y << ")" << std::endl;
    }


    //// Binary operators ////

    inline Vec2 operator + (const Vec2& a, const Vec2& b)
    {
        return Vec2(a.x + b.x, a.y + b.y);
    }


    inline void operator += (Vec2& a, const Vec2& b)
    {
        a.x += b.x;
        a.y += b.y;
    }


    inline Vec2 operator - (const Vec2& a, const Vec2& b)
    {
        return Vec2(a.x - b.x, a.y - b.y); 
    }


    inline void operator -= (Vec2& a, const Vec2& b)
    {
        a.x -= b.x;
        a.y -= b.y;
    }


    inline Vec2 operator * (const float& a, const Vec2& b)
    {
        return Vec2(a * b.x, a * b.y);
    }


    inline Vec2 operator * (const Vec2& a, const float& b)
    {
        return Vec2(b * a.x, b * a.y);
    }


    inline void operator *= (Vec2& a, const float& b)
    {
        a.x *= b;
        a.y *= b;
    }


    //// Comparison operators ////

    inline bool operator == (const Vec2& a, const Vec2& b)
    {
        return (a.x == b.x && a.y == b.y);
    }


    inline bool operator != (const Vec2& a, const Vec2& b)
    {
        return !(a == b);
    }
    
}