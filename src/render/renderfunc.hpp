#pragma once

#include "util/data/size2d.hpp"
#include "render/rendertarget.hpp"


/* renderfunc - 3D rendering functionality. */
namespace Base
{
    EXPORT void setRenderTarget(RenderTarget* target);
    EXPORT RenderTarget* getRenderTarget();
}