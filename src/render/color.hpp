#pragma once

#include <stdio.h> // sscanf

#include "util/mathfunc.hpp"


namespace Base
{
    /* A color with a Red, Green, Blue and (optional) Alpha value. */
    struct Color
    {
        float r, g, b, a;

        Color() {}
        
        Color(int red, int green, int blue, int alpha = 255)
        {
            this->r = (float)red / 255.f;
            this->g = (float)green / 255.f;
            this->b = (float)blue / 255.f;
            this->a = (float)alpha / 255.f;
        }

        Color(float value, float alpha = 1.f)
        {
            r = g = b = value;
            this->a = alpha;
        }

        Color(float red, float green, float blue, float alpha = 1.f)
        {
            this->r = red;
            this->g = green;
            this->b = blue;
            this->a = alpha;
        }

        Color(const Color& color, float alpha = 1.f)
        {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a * alpha;
        }

        inline Color(float arr[3])
        {
            r = arr[0];
            g = arr[1];
            b = arr[2];
            a = 1.f;
        }

        Color(const string& hex)
        {
            string nHex = hex;

            if (nHex[0] == '#')
                nHex = hex.substr(1, hex.length() - 1);

            int iR, iG, iB, iA;
            iR = iG = iB = 0;
            iA = 255;
            
            switch (hex.length() / 2)
            {
                case 1:  sscanf(&nHex[0], "%02x", &iR); iG = iB = iR;               break;
                case 3:  sscanf(&nHex[0], "%02x%02x%02x", &iR, &iG, &iB);           break;
                case 4:  sscanf(&nHex[0], "%02x%02x%02x%02x", &iR, &iG, &iB, &iA);  break;
            }

            r = iR / 255.f;
            g = iG / 255.f;
            b = iB / 255.f;
            a = iA / 255.f;
        }

        // Binary operators

        inline Color& operator = (const Color& other)
        {
            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;
            return *this;
        }

        inline Color operator * (float mul) const
        {
            return Color(mul * r, mul * g, mul * b, a);
        }

        inline Color operator + (const Color& other) const
        {
            return Color(
                min(1.f, r + other.r),
                min(1.f, g + other.g),
                min(1.f, b + other.b),
                min(1.f, a + other.a)
            );
        }

        inline Color operator * (const Color& other) const
        {
            return Color(r * other.r, g * other.g, b * other.b, a * other.a);
        }

        // Comparison operators

        inline bool operator == (const Color& other) const
        {
            return (approxEq(r, other.r) && approxEq(g, other.g) && approxEq(b, other.b) && approxEq(a, other.a));
        }

        inline bool operator != (const Color& other) const
        {
            return !(*this == other);
        }
    };

    inline std::ostream& operator << (std::ostream& cout, const Color& a)
    {
        return cout << "(" << a.r << "," << a.g << "," << a.b << "," << a.a << ")" << endl;
    }

    inline void operator += (Color& a, const Color& b)
    {
        a = a + b;
    }

    inline Color operator * (float a, const Color& b)
    {
        return Color(a * b.r, a * b.g, a * b.b, b.a);
    }

    inline void operator *= (Color& a, const Color& b)
    {
        a = a * b;
    }

    inline void operator *= (Color& a, float b)
    {
        a = a * b;
    }
}