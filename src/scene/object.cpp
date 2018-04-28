#include "common.hpp"
#include "scene/object.hpp"

Base::Object::Object()
{
    resetTransform();
}

Base::Object* Base::Object::translate(const Vec3f& translate)
{
    pos += translate;
    matrix = Mat4f::translate(translate) * matrix;
    return this;
}

Base::Object* Base::Object::scale(const Vec3f& scale)
{
    sca *= scale;
    matrix = Mat4f::scale(scale) * matrix;
    return this;
}

void Base::Object::resetTransform()
{
    pos = { 0.f, 0.f, 0.f };
    rot = { 0.f, 0.f, 0.f };
    sca = { 1.f };
    matrix = Mat4f::identity();
}

void Base::Object::buildMatrix()
{
    matrix = Mat4f::translate(pos) *
            // Mat4f::rotate({ 0.f, 1.f, 0.f }, rot.y) *
             Mat4f::scale(sca);
}