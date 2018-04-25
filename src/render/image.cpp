#include "SOIL.h"
#define GLEW_STATIC
#include "GL/glew.h"

#include "common.hpp"
#include "render/image.hpp"
#include "util/stringfunc.hpp"


EXPORT Base::Image::Image(string filename)
{
    uchar *pixelData = SOIL_load_image(&stringToWstring(filename)[0], &glTextureSize.width, &glTextureSize.height, 0, SOIL_LOAD_RGBA);
    load(pixelData);
}

EXPORT Base::Image::Image(const Data& data)
{
    uchar *pixelData = SOIL_load_image_from_memory((uchar*)data.ptr, data.size, &glTextureSize.width, &glTextureSize.height, 0, SOIL_LOAD_RGBA);
    load(pixelData);
}

EXPORT Base::Image::Image(Color color)
{
    glTextureSize = { 1, 1 };
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &color);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Base::Image::load(uchar* pixelData)
{
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glTextureSize.width, glTextureSize.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(pixelData);
}

bool Base::Image::reload(const Data& data)
{
    glDeleteTextures(1, &glTexture);
    uchar *pixelData = SOIL_load_image_from_memory((uchar*)data.ptr, data.size, &glTextureSize.width, &glTextureSize.height, 0, SOIL_LOAD_RGBA);
    load(pixelData);
    return true;
}