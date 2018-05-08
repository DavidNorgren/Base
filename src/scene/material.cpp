#include "common.hpp"
#include "scene/material.hpp"
#include "apphandler.hpp"


EXPORT Base::Image* Base::Material::getTexture()
{
    if (texture)
        return texture;

    return appHandler->solidColor;
}