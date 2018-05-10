#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/data/list.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec3.hpp"
#include "util/data/vertex3d.hpp"
#include "resource/shader.hpp"
#include "scene/volume.hpp"


namespace Base
{
    /* A container for 3D triangle data. */
    class Shader;
    class TriangleMesh
    {
      public:
        EXPORT TriangleMesh();
        EXPORT TriangleMesh(List<Vertex3Df> vertexData, List<uint> indexData);
        EXPORT ~TriangleMesh();

        /* Adds a single vertex and returns its index. */
        EXPORT uint addVertex(Vertex3Df vertex);

        /* Adds a set of three indices to previously added vertices.
           If negative numbers are supplied, they will be relative to the
           indices of the last added vertices. */
        EXPORT void addTriangle(const Vec3i& indices);

        /* Adds a single index reference to a vertex. If a negative
           number is supplied, it will be relative to the indices of
           the last added vertices. if newTriangle is true, a new triangle
           will be added using this index and two previous indices. */
        EXPORT void addIndex(int index, bool newTriangle = false);

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

        /* Clears all triangle data. */
        EXPORT void clear();

        /* Finalizes the mesh for rendering and sets the bounding box. */
        EXPORT void update();

        /* Returns the axis aligned bounding box. */
        EXPORT const AABB& getAxisAlignedBox() const { return axisAlignedBox; }
      
      friend class Shader;
      protected:
        List<Vertex3Df> vertexData;
        List<uint> indexData;
        GLuint glVbo, glIbo;
        AABB axisAlignedBox;
    };

    /* Basic shapes (origin in center) */
    
    class Cube : public TriangleMesh
    {
      public:
        EXPORT Cube(const Size3Df& size, const Vec2f& texRepeat = { 1.f });
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