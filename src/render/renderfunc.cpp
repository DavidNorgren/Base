#include "common.hpp"
#include "render/renderfunc.hpp"


Base::RenderTarget* renderTarget = nullptr;

EXPORT void Base::setRenderTarget(Base::RenderTarget* target)
{
    target->set();
    renderTarget = target;
}

EXPORT Base::RenderTarget* Base::getRenderTarget()
{
    return renderTarget;
}