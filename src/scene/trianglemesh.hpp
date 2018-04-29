#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "resource/image.hpp"
#include "resource/shader.hpp"
#include "scene/object.hpp"
#include "scene/material.hpp"
#include "util/data/list.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec2.hpp"
#include "util/data/vec3.hpp"
#include "util/data/vertex3d.hpp"


namespace Base
{
    class TriangleMesh : public Object
    {
      public:
        TriangleMesh() : Object() {};
        TriangleMesh(Material* material) : material(material), Object() {}
        EXPORT ~TriangleMesh();

        void render(Shader* shader, const Mat4f& projMat) const override;

        /* Adds a single vertex and returns its index. */
        uint addVertex(Vertex3Df vertex);

        /* Adds a set of three vertex indices to represent a triangle. */
        void addTriangle(const Vec3ui& indices);

        /* Adds a single index reference to a vertex. */
        void addIndex(uint index);

        /* Adds a set of three vertices to represent a triangle.
           Indices are automatically generated and added. */
        void addTriangle(Vertex3Df v1, Vertex3Df v2, Vertex3Df v3);

        /* Calculates the normals from the triangle data. */
        void setNormals();

        /* Returns the index of a vertex with the given values.
           Gives -1 if it could not be found. */
        int getVertexIndex(const Vertex3Df& vertex);

        int getTriangleCount();

        void update();
      
      protected:
        List<Vertex3Df> vertexData;
        List<uint> indexData;
        GLuint glVbo, glIbo;
        Material* material;
    };

    // Basic shapes (origin in center)

    class Cube : public TriangleMesh
    {
      public:
        Cube(const Size3Df& size, Material* material);
    };

    class Plane : public TriangleMesh
    {
      public:
        Plane(const Size2Df& size, Material* material, const Vec2f& textureRepeat = { 1.f });
    };

    class Sphere : public TriangleMesh
    {
      public:
        Sphere(float radius, int detail, Material* material);
    };
}