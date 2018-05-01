#define GLEW_STATIC
#include "GL/glew.h"

#include "common.hpp"
#include "scene/trianglemesh.hpp"


EXPORT Base::TriangleMesh::TriangleMesh()
{
    glGenBuffers(1, &glVbo);
    glGenBuffers(1, &glIbo);
}

EXPORT Base::TriangleMesh::TriangleMesh(List<Vertex3Df> vertexData, List<uint> indexData) : TriangleMesh()
{
    this->vertexData = vertexData;
    this->indexData = indexData;
    update();
}

EXPORT Base::TriangleMesh::~TriangleMesh()
{
    glDeleteBuffers(1, &glVbo);
    glDeleteBuffers(1, &glIbo);
}

EXPORT uint Base::TriangleMesh::addVertex(Vertex3Df vertex)
{
    vertexData.add(vertex);
    return vertexData.size() - 1;
}

EXPORT void Base::TriangleMesh::addTriangle(const Vec3ui& indices)
{
    indexData.add(indices[0]);
    indexData.add(indices[1]);
    indexData.add(indices[2]);
}

EXPORT void Base::TriangleMesh::addIndex(uint index)
{
    indexData.add(index);
}

EXPORT void Base::TriangleMesh::addTriangle(Vertex3Df v1, Vertex3Df v2, Vertex3Df v3)
{
    vertexData.add(v1);
    vertexData.add(v2);
    vertexData.add(v3);
    indexData.add((uint)(vertexData.size() - 3));
    indexData.add((uint)(vertexData.size() - 2));
    indexData.add((uint)(vertexData.size() - 1));
}

EXPORT void Base::TriangleMesh::setNormals()
{
    for (uint i = 0; i < indexData.size(); i += 3)
    {
        Vec3ui triangle = { indexData[i], indexData[i + 1], indexData[i + 2] };
        Vertex3Df* vertices[] = { &vertexData[triangle[0]], &vertexData[triangle[1]], &vertexData[triangle[2]] };

        // Normal of the triangle
        Vec3f normal = Vec3f::cross(
            vertices[1]->pos - vertices[0]->pos,
            vertices[2]->pos - vertices[0]->pos
        );

        // Add triangle normal to each vertex
        vertices[0]->normal += normal;
        vertices[1]->normal += normal;
        vertices[2]->normal += normal;
    }
    
    // Normalize
    for (uint i = 0; i < vertexData.size(); i++)
        vertexData[i].normal = vertexData[i].normal.normalize();
}

EXPORT void Base::TriangleMesh::clear()
{
    vertexData.clear();
    indexData.clear();
}

EXPORT void Base::TriangleMesh::update()
{
    // Update vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex3Df), &vertexData[0], GL_STATIC_DRAW);

    // Update index buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(uint), &indexData[0], GL_STATIC_DRAW);
}

EXPORT int Base::TriangleMesh::getVertexIndex(const Vertex3Df& vertex)
{
    int index;
    for (index = vertexData.size() - 1; index > -1; index--)
        if (vertexData[index] == vertex)
            break;

    return index;
}

EXPORT int Base::TriangleMesh::getTriangleCount()
{
    return indexData.size() / 3;
}

EXPORT Base::Plane::Plane(const Base::Size2Df& size, const Vec2f& texRepeat)
{
    Vec3f normal = { 0, 1, 0 };
    vertexData = {
        { { -size.width / 2.f, 0.f, -size.height / 2.f }, { 0.f, 0.f },                     normal },
        { {  size.width / 2.f, 0.f, -size.height / 2.f }, { texRepeat.x, 0.f },           normal },
        { {  size.width / 2.f, 0.f,  size.height / 2.f }, { texRepeat.x, texRepeat.y }, normal },
        { { -size.width / 2.f, 0.f,  size.height / 2.f }, { 0.f, texRepeat.y },           normal }
    };

    indexData = {
        2, 1, 0,
        0, 3, 2
    };

    update();
}