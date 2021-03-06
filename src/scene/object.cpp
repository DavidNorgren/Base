#include "common.hpp"
#include "scene/object.hpp"


EXPORT Base::Object::Object(const string& name)
{
    this->name = name;
    resetTransform();
}

EXPORT Base::Object::Object(Base::Model* model, const string& name) : Object(name)
{
    this->model = model;
}

EXPORT void Base::Object::render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const
{
    if (model)
        model->render(shader, matM * matrix, matVP);
}

EXPORT Base::Object* Base::Object::translate(const Vec3f& translate)
{
    pos += translate;
    return this;
}

EXPORT Base::Object* Base::Object::scale(const Vec3f& scale)
{
    sca *= scale;
    return this;
}

EXPORT Base::Object* Base::Object::rotate(const Vec3f& angles)
{
    rot += angles;
    return this;
}

EXPORT Base::Object* Base::Object::rotateX(float angle)
{
    rot.x += angle;
    return this;
}

EXPORT Base::Object* Base::Object::rotateY(float angle)
{
    rot.y += angle;
    return this;
}

EXPORT Base::Object* Base::Object::rotateZ(float angle)
{
    rot.z += angle;
    return this;
}

EXPORT void Base::Object::resetTransform()
{
    pos = { 0.f, 0.f, 0.f };
    rot = { 0.f, 0.f, 0.f };
    sca = { 1.f };
    matrix = Mat4f::identity();
}

EXPORT Base::Object* Base::Object::buildMatrix()
{
    matrix = Mat4f::translate(pos) *
             Mat4f::rotate({ 1.f, 0.f, 0.f }, rot.x) *
             Mat4f::rotate({ 0.f, 1.f, 0.f }, rot.y) *
             Mat4f::rotate({ 0.f, 0.f, 1.f }, rot.z) *
             Mat4f::scale(sca);

    box = BoundingBox(model->getAxisAlignedBox(), matrix);
    return this;
}