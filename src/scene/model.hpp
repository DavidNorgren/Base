#pragma once

#include "util/data/mat4.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"
#include "resource/shader.hpp"


namespace Base
{
    /* A collection of meshes with materials. */
    class Model
    {
      public:
        EXPORT Model() {}
        EXPORT Model(TriangleMesh* mesh, Material* material = nullptr);
        
        void render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const;

      protected:
        List<TriangleMesh*> meshes;
        List<Material*> materials;
    };
}