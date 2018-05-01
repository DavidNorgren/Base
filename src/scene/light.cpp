#include "common.hpp"
#include "scene/light.hpp"


Base::Light::Light()
{
    size = { 2048, 2048 };
    
    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &glDepthTexture);
    glBindTexture(GL_TEXTURE_2D, glDepthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    Color borderColor(1.f);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, glDepthTexture, 0);

    // No color buffer is drawn to.
    glDrawBuffer(GL_NONE);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Base::Light::~Light()
{
    glDeleteFramebuffers(1, &glFramebuffer);
    glDeleteTextures(1, &glDepthTexture);
}

void Base::Light::setPosition(const Vec3f& position)
{
    pos = position;
    dir = pos.normalize();
}

Base::Light* Base::Light::translate(const Vec3f& translate)
{
    pos += translate;
    dir = pos.normalize();
    return this;
}

void Base::Light::buildMatrix(float ratio)
{
    static Mat4f matBias = Mat4f(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    matP = Mat4f::ortho(
        -orthoSize.x / 2.f, orthoSize.x / 2.f,
        -(orthoSize.y / 2.f) / ratio, (orthoSize.y / 2.f) / ratio,
        -orthoSize.z / 2.f, orthoSize.z / 2.f
    );
    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    matPV = matP * matV;
    matBiasPV = matBias * matPV;
}