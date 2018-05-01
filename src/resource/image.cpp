#include "SOIL.h"
#define GLEW_STATIC
#include "GL/glew.h"

#include "common.hpp"
#include "resource/image.hpp"
#include "util/stringfunc.hpp"


constexpr bool genMipmap = true;
constexpr uint genMipmapLevels = 8;

EXPORT Base::Image::Image(const Color& color)
{
    size = { 1, 1 };
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &color);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Base::Image::load(const FilePath& file)
{
    uchar *pixelData = SOIL_load_image(&stringToWstring(file.getFullPath())[0], &size.width, &size.height, 0, SOIL_LOAD_RGBA);
    load(pixelData);
}

void Base::Image::load(const FileData& data)
{
    uchar *pixelData = SOIL_load_image_from_memory((uchar*)&data[0], data.size(), &size.width, &size.height, 0, SOIL_LOAD_RGBA);
    load(pixelData);
}

void Base::Image::load(uchar* pixelData)
{
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if (genMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexStorage2D(GL_TEXTURE_2D, genMipmapLevels, GL_RGBA8, size.width, size.height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(pixelData);
}

void Base::Image::cleanUp()
{
    glDeleteTextures(1, &glTexture);  
}