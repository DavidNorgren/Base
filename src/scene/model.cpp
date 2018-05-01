#include "common.hpp"
#include "scene/model.hpp"


EXPORT Base::Model::Model(Base::TriangleMesh* mesh, Base::Material* material)
{
    meshes.add(mesh);
    materials.add(material);
}

void Base::Model::render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const
{
    for (uint i = 0; i < meshes.size(); i++)
        shader->render3D(matM, matVP, meshes[i], materials[i]);
}