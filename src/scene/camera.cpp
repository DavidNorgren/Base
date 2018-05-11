#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


EXPORT void Base::Camera::buildMatrix(float ratio)
{
    this->ratio = ratio;

    float tFov = dtan(fov / 2.f);
    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    matP = Mat4f::perspective(tFov, ratio, zNear, zFar);
    matVP = matP * matV;

    // Update frustum
    buildFrustum();
    frustumSize = 2.f * zNear * tFov * ratio;
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

EXPORT bool Base::Camera::boxVisible(const BoundingBox& box) const
{
    for (uint i = 0; i < 6; i++)
    {
        // If all of the points lie behind one of the
        // planes, the box is not rendered.
        bool pointInside = false;
        for (uint j = 0; j < 8; j++)
        {
            if (Vec4f::dot(frustum[i], box[j]) > 0.f)
            {
                pointInside = true;
                break;
            }
        }

        if (!pointInside)
            return false;
    }

    return true;
}

EXPORT bool Base::Camera::pointVisible(const Base::Vec4f& point) const
{
    // The point must lie within all six planes of the frustum
    for (uint i = 0; i < 6; i++)
        if (Vec4f::dot(frustum[i], point) < 0.f)
            return false;

    return true;
}