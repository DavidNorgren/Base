#include "common.hpp"
#include "render/renderfunc.hpp"


Base::RenderTarget* renderTarget = nullptr;

EXPORT void Base::setRenderTarget(Base::RenderTarget* target)
{
    Size2Di size = target->getSize();
    glBindFramebuffer(GL_FRAMEBUFFER, target->getGlFramebuffer());
    glViewport(0, 0, size.width, size.height);
    renderTarget = target;
}

EXPORT Base::RenderTarget* Base::getRenderTarget()
{
    return renderTarget;
}