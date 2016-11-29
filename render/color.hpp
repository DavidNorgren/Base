#pragma once

#include <iostream>

#include "util/math/mathfunc.hpp"

#define COLOR_BLACK     Color(0.f)
#define COLOR_WHITE     Color(1.f)
#define COLOR_RED       Color(1.f, 0.f, 0.f)
#define COLOR_LIME      Color(0.f, 1.f, 0.f)
#define COLOR_BLUE      Color(0.f, 0.f, 1.f)
#define COLOR_GREEN     Color(0.f, 0.8f, 0.f)


namespace Base
{

    /* RGBA color */
    struct Color
    {
        float r, g, b, a;

        //// Constructors ////

        Color()
        {
            r = g = b = 0.f;
            a = 1.f;
        }


        Color(int r, int g, int b, int a = 255)
        {
            this->r = (float)r / 255.f;
            this->g = (float)g / 255.f;
            this->b = (float)b / 255.f;
            this->a = (float)a / 255.f;
        }


        Color(float v)
        {
            r = g = b = v;
            a = 1.f;
        }


        Color(float r, float g, float b, float a = 1.f)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }


        Color(const Color& color)
        {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }


        Color(float arr[3])
        {
            r = arr[0];
            g = arr[1];
            b = arr[2];
            a = 1.f;
        }
    };


    //// Unary operators ////

    inline std::ostream& operator << (std::ostream& cout, const Color& a)
    {
        return cout << "(" << a.r << "," << a.g << "," << a.b << "," << a.a << ")" << std::endl;
    }


    //// Binary operators ////

    inline Color operator + (const Color& a, const Color& b)
    {
        return Color(
            min(1.f, a.r + b.r),
            min(1.f, a.g + b.g),
            min(1.f, a.b + b.b),
            min(1.f, a.a + b.a)
        );
    }


    inline void operator += (Color& a, const Color& b)
    {
        a = a + b;
    }


    inline Color operator * (const Color& a, const Color& b)
    {
        return Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
    }


    inline Color operator * (float a, const Color& b)
    {
        return Color(a * b.r, a * b.g, a * b.b, b.a);
    }


    inline Color operator * (const Color& a, float b)
    {
        return Color(b * a.r, b * a.g, b * a.b, a.a);
    }


    inline void operator *= (Color& a, const Color& b)
    {
        a = a * b;
    }


    inline void operator *= (Color& a, float b)
    {
        a = a * b;
    }


    //// Comparison operators ////

    inline bool operator == (const Color& a, const Color& b)
    {
        return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
    }


    inline bool operator != (const Color& a, const Color& b)
    {
        return !(a == b);
    }

}