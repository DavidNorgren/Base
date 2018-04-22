#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


EXPORT Base::Mat4f Base::Camera::getMatrix(float ratio)
{
	static float i = 0.f;
    // DEBUG!
	pos = { -58.7811f, 61.4856f, 42.2966f };
	xaxis = { 0.805777f, 0.0269571f, 0.591605f };
	yaxis = { 0.379608f, 0.743242f, -0.550899f };
	zaxis = { 0.454557f, -0.668479f, -0.588655f };
    fov = 90.f;

	Mat4f P = Mat4f::perspective(dtan(fov / 2.f), ratio, 0.1f, 10000.f);
	Mat4f V = Mat4f::view(xaxis, yaxis, zaxis) * Mat4f::translate(-pos);
	return P * V;
}