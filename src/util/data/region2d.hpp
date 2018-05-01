#pragma once

#include "util/data/vec2.hpp"
#include "util/data/size2d.hpp"
#include "util/mathfunc.hpp"


namespace Base
{
    template<typename T> struct Region2D
    {
        union
        {
            Vec2<T> pos;
            struct { T x, y; };
        };
        union
        {
            Size2D<T> size;
            struct { T width, height; };
        };

        Region2D() {}
        
        Region2D(const Vec2<T>& pos, const Size2D<T>& size)
        {
            this->pos = pos;
            this->size = size;
        }

        Region2D(T x, T y, T width, T height)
        {
            pos.x = x;
            pos.y = y;
            size.width = width;
            size.height = height;
        }
        
        inline Region2D(const Region2D& other)
        {
            pos = other.pos;
            size = other.size;
        }
        
        static inline Region2D intersection(const Region2D& a, const Region2D& b)
        {
            Vec2<T> start = {
                max(a.pos.x, b.pos.x),
                max(a.pos.y, b.pos.y)
            };
            Vec2<T> end = {
                min(a.pos.x + a.width, b.pos.x + b.width),
                min(a.pos.y + a.height, b.pos.y + b.height)
            };

            return {
                start,
                Size2D<T>(max(0, end.x - start.x), max(0, end.y - start.y))
            };
        }

        inline Region2D& operator = (const Region2D& other)
        {
            pos = other.pos;
            size = other.size;
            return *this;
        }
    };
    
    template<typename T> inline std::ostream& operator << (std::ostream& out, const Region2D<T>& region)
    {
        return out << "(Position: " << region.pos << ", Size:" << region.size << ")" << endl;
    }

    // Define shorthands
    using Region2Di = Region2D<int>;
    using Region2Df = Region2D<float>;
    using Region2Dd = Region2D<double>;
    using ScreenArea = Region2D<int>;
}