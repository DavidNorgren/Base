#include "common.hpp"
#include "scene/material.hpp"


EXPORT Base::Image* Base::Material::getTexture() const
{
    return (diffuseTexture ? diffuseTexture : Image::getSingleColor(Colors::WHITE));
}

EXPORT Base::Image* Base::Material::getAmbientTexture() const
{
    return (ambientTexture ? ambientTexture : Image::getSingleColor(Colors::WHITE));
}

EXPORT Base::Image* Base::Material::getDiffuseTexture() const
{
    return (diffuseTexture ? diffuseTexture : Image::getSingleColor(Colors::WHITE));
}

EXPORT Base::Image* Base::Material::getSpecularTexture() const
{
    return (specularTexture ? specularTexture : Image::getSingleColor(Colors::BLACK));
}

EXPORT Base::Image* Base::Material::getSpecularExpTexture() const
{
    return (specularExpTexture ? specularExpTexture : Image::getSingleColor(Colors::BLACK));
}

EXPORT Base::Image* Base::Material::getOpacityTexture() const
{
    return (opacityTexture ? opacityTexture : Image::getSingleColor(Colors::BLACK));
}

EXPORT Base::Image* Base::Material::getBumpTexture() const
{
    return (bumpTexture ? bumpTexture : Image::getSingleColor(Colors::BLACK));
}