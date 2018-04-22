#define GLEW_STATIC
#include "GL/glew.h"

#include "common.hpp"
#include "scene/trianglemesh.hpp"

EXPORT Base::TriangleMesh::~TriangleMesh()
{
	glDeleteBuffers(1, &glVbo);
	glDeleteBuffers(1, &glIbo);
}

void Base::TriangleMesh::render(Shader* shader, Mat4f projMat)
{
    shader->render3D(projMat, glVbo, vertexData.size(), glIbo, indexData.size() * 3, texture->glTexture);
}

void Base::TriangleMesh::setBuffers()
{
	// Set up VBO (Vertex buffer object)
	glGenBuffers(1, &glVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glVbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex3Df), &vertexData[0], GL_STATIC_DRAW);

	// Set up IBO (Index buffer object)
	glGenBuffers(1, &glIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(Vec3ui), &indexData[0], GL_STATIC_DRAW);
}

Base::Plane::Plane(Base::Size2Df size, Base::Image* texture)
{
    this->texture = texture;

    Vec3f normal = { 0, 1, 0 };

    vertexData = {
        { { -size.width / 2.f, 0.f, -size.height / 2.f }, normal, { 0, 0 }},
        { {  size.width / 2.f, 0.f, -size.height / 2.f }, normal, { 1, 0 }},
        { {  size.width / 2.f, 0.f,  size.height / 2.f }, normal, { 1, 1 }},
        { { -size.width / 2.f, 0.f,  size.height / 2.f }, normal, { 0, 1 }}
    };

    indexData = {
        { 0, 1, 2 } , { 2, 3, 0 }
    };

    setBuffers();
}