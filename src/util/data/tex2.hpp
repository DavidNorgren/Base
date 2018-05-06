#pragma once

#include "util/data/vec2.hpp"


namespace Base
{
    /* OpenGL uses 0,0 as the bottom-left texture pixel, so this class
       performs the conversion from the more universal top-left layout. */
    template<typename T> struct Tex2 : public Vec2<T>
    {
        // Constructors

        Tex2() {}

        Tex2(T x)
        {
            this->u = x;
            this->v = (T)1 - x;
        }
        
        Tex2(T u, T v)
        {
            this->u = u;
            this->v = (T)1 - v;
        }

        Tex2(T x[2])
        {
            this->u = x[0];
            this->v = (T)1 - x[1];
        }
    };

    // Define shorthands
    
    using Tex2f = Tex2<float>;
    using Tex2d = Tex2<double>;
}