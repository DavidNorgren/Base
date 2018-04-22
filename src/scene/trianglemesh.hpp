#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "render/image.hpp"
#include "render/shader.hpp"
#include "scene/object.hpp"
#include "util/data/list.hpp"
#include "util/data/mat4.hpp"
#include "util/data/size2d.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec3.hpp"
#include "util/data/vertex3d.hpp"


namespace Base
{
    class TriangleMesh : public Object
    {
      public:
        void render(Shader* shader, Mat4f projMat) override;
        EXPORT ~TriangleMesh();
      
      protected:
        void setBuffers();
        List<Vertex3Df> vertexData;
        List<Vec3ui> indexData;
    	  GLuint glVbo, glIbo;
    };

    // Basic shapes (origin in center)

    class Cube : public TriangleMesh
    {
      public:
        Cube(Size3Df size, Image* texture);
    };

    class Plane : public TriangleMesh
    {
      public:
        Plane(Size2Df size, Image* texture);
    };

    class Sphere : public TriangleMesh
    {
      public:
        Sphere(float radius, int detail, Image* texture);
    };
}