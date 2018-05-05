#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


void Base::Camera::buildMatrix(float ratio)
{
    this->ratio = ratio;

    float tanVfov = dtan(fov / 2.f);
    float tanHfov = dtan((fov * ratio) / 2.f);

    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    matP = Mat4f::perspective(tanVfov, ratio, zNear, zFar);
    matVP = matP * matV;

    // Update frustum
    buildFrustum();
}

void Base::Camera::buildFrustum()
{
    frustum[0] = {  1, 0, 0, 1 };
    frustum[1] = { -1, 0, 0, 1 };
    frustum[2] = { 0,  1, 0, 1 };
    frustum[3] = { 0, -1, 0, 1 };
    frustum[4] = { 0, 0,  1, 1 };
    frustum[5] = { 0, 0, -1, 1 };
    
    Mat4f matVPt = matVP.transpose();
    for (uint i = 0; i < 6; i++)
    {
        Vec4f mul = matVPt * frustum[i];
        frustum[i] = mul / Vec3f(mul.x, mul.y, mul.z).length();
    }
}

bool Base::Camera::boxVisible(const BoundingBox& box) const
{
    // If one of the 8 points is visible, the box counts as visible
    for (uint i = 0; i < 8; i++)
        if (pointVisible(box[i]))
            return true;

    return false;
}

bool Base::Camera::pointVisible(const Base::Vec3f& point) const
{
    // The point must lie within all six planes of the frustum
    for (uint i = 0; i < 6; i++)
        if (Vec4f::dot(frustum[i], Vec4f(point)) < 0.f)
            return false;

    return true;
}