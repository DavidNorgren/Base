#define GLEW_STATIC
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common.hpp"
#include "resource/sprite.hpp"
#include "util/stringfunc.hpp"

constexpr bool genMipmap = true;
constexpr uint genMipmapLevels = 8;


void Base::Sprite::load(const FilePath& file)
{
    int channels;
    uchar *pixelData = stbi_load(file.getFullPath().c_str(), &size.width, &size.height, &channels, 4);
    load(pixelData);
}

void Base::Sprite::load(const FileData& data)
{
    int channels;
    uchar *pixelData = stbi_load_from_memory((const uchar*)&data[0], data.size(), &size.width, &size.height, &channels, 4);
    load(pixelData);
}

void Base::Sprite::load(const uchar* pixelData)
{
    // Convert to OpenGL format (flip along Y)
    for (uint x = 0; x < size.x; x++)
        for (uint y = 0; y < size.y / 2; y++)
            std::swap(
                ((int32_t*)pixelData)[x + y * size.x],
                ((int32_t*)pixelData)[x + (size.y - 1 - y) * size.x]
            );
    
    // Create OpenGL texture
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
}

void Base::Sprite::cleanUp()
{
    glDeleteTextures(1, &glTexture);  
}