#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/data/list.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec3.hpp"
#include "util/data/vertex3d.hpp"
#include "resource/shader.hpp"


namespace Base
{
    /* A container for 3D vertex data. */
    class Shader;
    class TriangleMesh
    {
      friend class Shader;

      public:
        EXPORT TriangleMesh();
        EXPORT TriangleMesh(List<Vertex3Df> vertexData, List<uint> indexData);
        EXPORT ~TriangleMesh();

        /* Adds a single vertex and returns its index. */
        EXPORT uint addVertex(Vertex3Df vertex);

        /* Adds a set of three indices to previously added vertices. */
        EXPORT void addTriangle(const Vec3ui& indices);

        /* Adds a single index reference to a vertex. */
        EXPORT void addIndex(uint index);

        /* Adds a set of three vertices to represent a triangle.
           Indices are automatically generated and added. */
        EXPORT void addTriangle(Vertex3Df v1, Vertex3Df v2, Vertex3Df v3);

        /* Calculates the normals from the triangle data.
           This method assumes that all normals are set to { 0, 0, 0 }. */
        EXPORT void setNormals();

        /* Returns the index of a vertex with the given values.
           Gives -1 if it could not be found. */
        EXPORT int getVertexIndex(const Vertex3Df& vertex);

        /* Returns the amount of triangles in the mesh. */
        EXPORT int getTriangleCount();

        /* Finalizes the mesh for rendering. */
        EXPORT void update();
      
      protected:
        List<Vertex3Df> vertexData;
        List<uint> indexData;
        GLuint glVbo, glIbo;
    };

    /* Basic shapes (origin in center) */
    
    class Cube : public TriangleMesh
    {
      public:
        EXPORT Cube(const Size3Df& size);
    };

    class Plane : public TriangleMesh
    {
      public:
        EXPORT Plane(const Size2Df& size, const Vec2f& texRepeat = { 1.f });
    };

    class Sphere : public TriangleMesh
    {
      public:
        EXPORT Sphere(float radius, int detail);
    };
}