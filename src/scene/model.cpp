#include "common.hpp"
#include "scene/model.hpp"


EXPORT Base::Model::Model(Base::TriangleMesh* mesh, Base::Material* material)
{
    meshes.add(mesh);
    materials.add(material);
    axisAlignedBox = mesh->getAxisAlignedBox();
    debugAABB = new Cube(axisAlignedBox.getSize());
    debugAABBmaterial = new Material(Image::createSingleColor(Color(255, 255, 0, 20)));
}

EXPORT void Base::Model::render(Shader* shader, const Mat4f& matM, const Mat4f& matVP) const
{
    for (uint i = 0; i < meshes.size(); i++)
    {
        shader->render3D(matM, matVP, meshes[i], materials[i]);
        //shader->render3D(matM * Mat4f::translate((axisAlignedBox.maxPos + axisAlignedBox.minPos) / 2)* Mat4f::scale({ 1.05 }), matVP, debugAABB, debugAABBmaterial);
    }
}