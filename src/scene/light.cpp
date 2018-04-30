#include "common.hpp"
#include "scene/light.hpp"


Base::Light* Base::Light::translate(const Vec3f& translate)
{
    pos += translate;
    dir = pos.normalize();
    return this;
}