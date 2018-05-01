#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


void Base::Camera::buildMatrix(float ratio)
{
    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }) * Mat4f::translate(-pos);
    matP = Mat4f::perspective(dtan(fov / 2.f), ratio, 1.f, 10000.f);
    matPV = matP * matV;
}