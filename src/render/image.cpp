#include "SOIL.h"
#define GLEW_STATIC
#include "GL/glew.h"

#include "common.hpp"
#include "render/image.hpp"
#include "util/stringfunc.hpp"


Base::Image::Image(string filename)
{
    void *data = SOIL_load_image(&stringToWstring(filename)[0], &glTextureSize.width, &glTextureSize.height, 0, SOIL_LOAD_RGBA);
    load(data);
    SOIL_free_image_data((uchar*)data);
}

Base::Image::Image(File* file)
{
    void *data = SOIL_load_image_from_memory((uchar*)file->rawData, file->size, &glTextureSize.width, &glTextureSize.height, 0, SOIL_LOAD_RGBA);
    load(data);
    SOIL_free_image_data((uchar*)data);
}

Base::Image::Image(Color color, int width, int height)
{
    this->glTextureSize.width = width;
    this->glTextureSize.height = height;
    
    uchar* data = new uchar[width * height * 4];
    for (int c = 0; c < width * height; c += 4)
    {
        data[c] = color.r * 255.f;
        data[c + 1] = color.g * 255.f;
        data[c + 2] = color.b * 255.f;
        data[c + 3] = color.a * 255.f;
    }
    
    load(data);
    delete data;
}

void Base::Image::load(void* data)
{
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glTextureSize.width, glTextureSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}