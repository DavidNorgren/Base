
#include "tiny_obj_loader.hpp"

#include "common.hpp"
#include "resource/model.hpp"


Base::Model::Model(const string& filename)
{

}

Base::Model::Model(const Data& data)
{
    std::vector<tinyobj::shape_t> fileShapes;
    std::vector<tinyobj::material_t> fileMaterials;
    string err;
    //std::istringstream is(string(file->rawData, file->size));
    //tinyobj::LoadObj(fileShapes, fileMaterials, err, is);

   /* if (!err.empty())
        throw ModelFileException("Load OBJ error: " + err + ".");

    if (fileShapes.empty())
        throw ModelFileException("Load OBJ error: No shapes found.");


    cout << fileShapes.size() << endl;*/
}

void Base::Model::render(Shader* shader, Mat4f projMat)
{
    for (TriangleMesh* mesh : meshes)
        mesh->render(shader, projMat);
}