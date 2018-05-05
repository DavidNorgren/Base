#include "common.hpp"
#include "render/image.hpp"


Base::Image::~Image()
{
    if (glTexture)
        glDeleteTextures(1, &glTexture);
}

Base::Image* Base::Image::createSingleColor(const Color& color)
{
    Image* image = new Image();
    image->size = { 1, 1 };

    // Create OpenGL texture
    glGenTextures(1, &image->glTexture);
    glBindTexture(GL_TEXTURE_2D, image->glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &color);
    glBindTexture(GL_TEXTURE_2D, 0);

    return image;
}