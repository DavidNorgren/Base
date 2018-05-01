#include "common.hpp"
#include "render/surface.hpp"


Base::Surface::Surface(Size2Di size)
{
    this->size = size;

    // Create texture
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_FLOAT, 0);
    
    // Create depth render buffer
    glGenRenderbuffers(1, &glDepthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, glDepthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);

    // Create framebuffer
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthRenderbuffer);
    
    GLenum drawBuf[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuf);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}