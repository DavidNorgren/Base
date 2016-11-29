#include "SOIL.h"

#include "util/math/mathfunc.hpp"
#include "render/image.hpp"


Base::Image::Image(wstring filename)
{
    void *data = SOIL_load_image(&filename[0], &width, &height, 0, SOIL_LOAD_RGBA);
    load(data);
    SOIL_free_image_data((uchar*)data);
}


Base::Image::Image(File* file)
{
    void *data = SOIL_load_image_from_memory((uchar*)file->rawData, file->size, &width, &height, 0, SOIL_LOAD_RGBA);
    load(data);
    SOIL_free_image_data((uchar*)data);
}


Base::Image::Image(Color color, int width, int height)
{
    this->width = width;
    this->height = height;
    
    uchar* data = new uchar[width * height * 4];
    for (int c = 0; c < width * height; c += 4) {
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
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}