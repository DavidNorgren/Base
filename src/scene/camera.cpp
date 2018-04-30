#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


EXPORT Base::Mat4f Base::Camera::getMatrix(float ratio) const
{
    //Mat4f P = Mat4f::ortho(-200.f, 200.f, -200.f / ratio, 200.f / ratio, -200.f, 1000.f);
    Mat4f P = Mat4f::perspective(dtan(90.f / 2.f), ratio, 1.f, 10000.f);
    Mat4f V = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }) * Mat4f::translate(-pos);
    //Mat4f V = Mat4f::view(xaxis, yaxis, zaxis) * Mat4f::translate(-pos);
    return P * V;
}