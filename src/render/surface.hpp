#pragma once

#include "render/image.hpp"
#include "render/rendertarget.hpp"


namespace Base
{
    /* A type of image that can be rendered to and modified during runtime. */
    class Surface : public RenderTarget
    {
      public:
        Surface();
        Surface(Size2Di size);

        void resize(Size2Di size);
        void clear();
    };
}