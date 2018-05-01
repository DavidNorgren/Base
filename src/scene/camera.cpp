#include "common.hpp"
#include "scene/camera.hpp"
#include "util/mathfunc.hpp"


void Base::Camera::buildMatrix(float ratio)
{
    float zNear = 1.f, zFar = 1000.f;
    float tanHfov = dtan(fov / 2.f);
    float tanVfov = dtan((fov / ratio) / 2.f);

    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }) * Mat4f::translate(-pos);
    matP = Mat4f::perspective(tanHfov, ratio, zNear, zFar);
    matVP = matP * matV;

    float xn = zNear * tanHfov;
    float yn = zNear * tanVfov;
    float xf = zFar  * tanHfov;
    float yf = zFar  * tanVfov;

    Vec4f frustum[8] = {
        { -xn, yn, -zNear, 1.0f },
        { -xn, -yn, -zNear, 1.0f },
        { xn, -yn, -zNear, 1.0f },
        { xn, yn, -zNear, 1.0f },
        { -xf, yf, -zFar, 1.0f },
        { -xf, -yf, -zFar, 1.0f },
        { xf, -yf, -zFar, 1.0f },
        { xf, yf, -zFar, 1.0f },
    };

    for (int i = 0; i < 8; i++)
        frustum[i] = matV.inverse() * frustum[i];

    // DEBUG
    if (!frustumModel)
    {
        frustumMesh = new TriangleMesh();
        frustumMaterial = new Material(Image::createSingleColor(Color(255, 0, 0, 128)));
        frustumModel = new Model(frustumMesh, frustumMaterial);
    }

    frustumMesh->clear();

    for (int i = 0; i < 8; i++)
        frustumMesh->addVertex(Vertex3Df(Vec3f(frustum[i].x, frustum[i].y, frustum[i].z)));

    // X-
    frustumMesh->addTriangle({0, 4, 5});
    frustumMesh->addTriangle({5, 1, 0});

    // X+
    frustumMesh->addTriangle({7, 3, 2});
    frustumMesh->addTriangle({2, 6, 7});

    // Y-
    frustumMesh->addTriangle({2, 1, 5});
    frustumMesh->addTriangle({5, 6, 2});
    
    // Y+
    frustumMesh->addTriangle({7, 4, 0});
    frustumMesh->addTriangle({0, 3, 7});

    // Z-
    frustumMesh->addTriangle({0, 1, 2});
    frustumMesh->addTriangle({2, 3, 0});

    // Z+
    frustumMesh->addTriangle({4, 7, 6});
    frustumMesh->addTriangle({6, 5, 4});

    frustumMesh->update();
}