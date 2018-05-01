#include "common.hpp"
#include "scene/light.hpp"


Base::ShadowMap::ShadowMap(Size2Di size)
{
    this->size = size;
    
    // Create depth texture
    Color borderColor(1.f);
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
    // Create framebuffer
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glTexture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, glTexture, 0);
    glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Base::Light::Light()
{
    // Create shadowmaps
    for (int i = 0; i < lightNumCascades; i++)
        shadowMaps.add(new ShadowMap({ 2048, 2048 }));
}

Base::Light::~Light()
{
    for (ShadowMap* map : shadowMaps)
        delete map;
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
    matVP = matP * matV;
    matBiasVP = matBias * matVP;
}