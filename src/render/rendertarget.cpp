#include "common.hpp"
#include "render/rendertarget.hpp"


EXPORT Base::RenderTarget::~RenderTarget()
{
    if (glFramebuffer)
        glDeleteFramebuffers(1, &glFramebuffer);
    if (glDepthRenderbuffer)
        glDeleteRenderbuffers(1, &glDepthRenderbuffer);
}

EXPORT void Base::RenderTarget::set()
{
    ortho2D = Mat4f::ortho(0.f, size.width, size.height, 0.f, 0.f, 1.f);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glViewport(0, 0, size.width, size.height);
}