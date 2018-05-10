#include "common.hpp"
#include "render/surface.hpp"


EXPORT Base::Surface::Surface()
{
    glGenTextures(1, &glTexture);
    glGenRenderbuffers(1, &glDepthRenderbuffer);
    glGenFramebuffers(1, &glFramebuffer);
}

EXPORT Base::Surface::Surface(Size2Di size, GLint internalFormat, GLint format) : Surface()
{
    this->internalFormat = internalFormat;
    this->format = format;
    resize(size);
}

EXPORT void Base::Surface::resize(Base::Size2Di size)
{
    if (this->size == size)
        return;
    
    this->size = size;

    // Create texture
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.width, size.height, 0, format, GL_FLOAT, 0);
    
    // Create depth render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, glDepthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);

    // Create framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthRenderbuffer);
    
    GLenum drawBuf[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuf);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}