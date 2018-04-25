#pragma once

#include "scene/object.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"
#include "file/resource.hpp"


namespace Base
{
    struct ModelFileException : public runtime_error
    {
        ModelFileException(string message) : runtime_error(message) {};
    };

    class Model : public Object, public Resource
    {
      public:
        Model(const Data& data);
        void render(Shader* shader, Mat4f projMat);

      private:
        List<TriangleMesh*> meshes;
        List<Material*> materials;
    };
}