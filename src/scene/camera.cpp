#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


EXPORT Base::Mat4f Base::Camera::getMatrix(float ratio)
{
    fov = 90.f;
    Mat4f P = Mat4f::perspective(dtan(fov / 2.f), ratio, 1.f, 1000.f);
    // Mat4f V = Mat4f::view(xaxis, yaxis, zaxis) * Mat4f::translate(-pos);
    Mat4f V = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }) * Mat4f::translate(-pos);
    return P * V;
}