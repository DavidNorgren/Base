#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


EXPORT Base::Mat4f Base::Camera::getMatrix(float ratio)
{
	static float d = 0.f;
	pos = { dcos(d) * 200.f, 100.f, dsin(d) * 200.f };
	d += 0.5f;

    fov = 90.f;

	Mat4f P = Mat4f::perspective(dtan(fov / 2.f), ratio, 0.1f, 10000.f);
	//Mat4f V = Mat4f::view(xaxis, yaxis, zaxis) * Mat4f::translate(-pos);
	Mat4f V = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }) * Mat4f::translate(-pos);
	return P * V;
}