#pragma once

#include "util/data/mat4.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"
#include "scene/volume.hpp"
#include "resource/shader.hpp"


namespace Base
{
    /* A collection of meshes with materials. */
    class Model
    {
      public:
        EXPORT Model() {}
        EXPORT Model(TriangleMesh* mesh, Material* material = nullptr);
        
        EXPORT void render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const;

        const AABB& getAxisAlignedBox() const { return axisAlignedBox; }

      protected:
        List<TriangleMesh*> meshes;
        List<Material*> materials;
        AABB axisAlignedBox;
        TriangleMesh* debugAABB;
        Material* debugAABBmaterial;
    };
}