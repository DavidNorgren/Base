
#include "common.hpp"
#include "resource/obj.hpp"
#include "resource/sprite.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"
#include "util/timer.hpp"
#include "apphandler.hpp"


void Base::Obj::load(const FilePath& file)
{
    List<string> lines = fileGetLines(file);
    load(lines);
}

void Base::Obj::load(const FileData& data)
{
    List<string> lines = stringGetLines(string(&data[0], data.size()));
    load(lines);
}

void Base::Obj::load(const List<string>& lines)
{
    List<Vec3f> objPositions, objNormals;
    List<Tex2f> objTexCoords;
    List<Vec3i> objIndices;
    List<uint> objFaceNumVertices;
    Map<uint, Material*> objMaterials;

    // todo OMP
    Timer t1("[Obj] " + name + " Parse");
    for (const string& line : lines)
    {
        std::stringstream lineStream(line);
        string command;
        lineStream >> command;

        // Blankspace or comments
        if (command == "#" || command == "")
            continue;

        // Load material library (mtlib filename)
        else if (command == "mtllib")
        {
            string name = loadReadString(lineStream);
            loadMaterials(
                (TextFile*)appHandler->resHandler->get(getFilePath(name).getFullPath())
            );
        }

        // Use material (usemtl name)
        if (command == "usemtl")
        {
            string name = loadReadString(lineStream);
            if (mtlMap.find(name) != mtlMap.end())
                objMaterials[objFaceNumVertices.size()] = mtlMap[name];
        }

         // Position (v x y z [w])
        else if (command == "v")
        {
            Vec3f pos;
            lineStream >> pos.x >> pos.y >> pos.z;
            objPositions.add(pos);
        }

         // Normal (vn x y z)
        else if (command == "vn")
        {
            Vec3f norm;
            lineStream >> norm.x >> norm.y >> norm.z;
            objNormals.add(norm);
        }

        // Texture coordinate (vt u v [w])
        else if (command == "vt")
        {
            Tex2f texCoord;
            lineStream >> texCoord.u >> texCoord.v;
            objTexCoords.add(texCoord);
        }

        // Index (f v1 v2 v3 ... vN for a N-sided polygon)
        // The indices are 1-based for now, with 0 representing left-out values
        else if (command == "f")
        {
            string vertexStr;
            uint v = 0;
            while (lineStream >> vertexStr)
            {
                Vec3i vertex;
                List<string> vertexSplit = stringSplit(vertexStr, "/");

                switch (vertexSplit.size())
                {
                    // (pos)
                    case 1:
                        vertex = {
                            std::stoi(vertexSplit[0]),
                            0,
                            0
                        };
                        break;

                    // (pos/tex)
                    case 2:
                        vertex = {
                            std::stoi(vertexSplit[0]),
                            std::stoi(vertexSplit[1]),
                            0
                        };
                        break;
                        
                    // (pos/[tex]/norm)
                    case 3:
                        vertex = {
                            std::stoi(vertexSplit[0]),
                            (vertexSplit[1] != "" ? std::stoi(vertexSplit[1]) : 0),
                            0
                        };
                        break;
                }

                // Convert relative (-) to absolute
                if (vertex[0] < 0)
                    vertex[0] = objPositions.size() + 1 + vertex[0];

                if (vertex[1] < 0)
                    vertex[1] = objTexCoords.size() + 1 + vertex[1];

                if (vertex[2] < 0)
                    vertex[2] = objNormals.size() + 1 + vertex[2];

                objIndices.add(vertex);
                v++;
            }

            objFaceNumVertices.add(v);
        }
    }
    t1.stopAndPrint();

    // No materials found, add an empty reference
    if (objMaterials.size() == 0)
        objMaterials[0] = nullptr;

    // Go through each face and generate the meshes,
    // separated by materials in the file
    Timer t2("[Obj] " + name + " Generate triangles");
    TriangleMesh* mesh = nullptr;
    uint fileVertIndex = 0;

    for (uint fileFaceIndex = 0; fileFaceIndex < objFaceNumVertices.size(); fileFaceIndex++)
    {
        // New material, create new mesh
        if (objMaterials.find(fileFaceIndex) != objMaterials.end())
        {
            // Finalize current mesh, if any
            if (mesh)
            {
                mesh->setNormals();
                mesh->update();
            }

            // Create new
            mesh = new TriangleMesh();
            meshes.add(mesh);
            materials.add(objMaterials[fileFaceIndex]);
        }

        // Process face
        uint num = objFaceNumVertices[fileFaceIndex];
        for (uint v = 0; v < num; v++, fileVertIndex++)
        {
            Vec3i vertIndices = objIndices[fileVertIndex];
            Vertex3Df vertex;

            // Set texture/normal index to position index if unset
            if (vertIndices[1] == 0)
                vertIndices[1] = vertIndices[0];

            if (vertIndices[2] == 0)
                vertIndices[2] = vertIndices[0];

            // Make zero-based and find values
            vertIndices[0]--;
            vertIndices[1]--;
            vertIndices[2]--;

            vertex.pos      = (vertIndices[0] < objPositions.size()) ? 
                                objPositions[vertIndices[0]] : Vec3f(0.f);
                
            vertex.texCoord = (vertIndices[1] < objTexCoords.size()) ? 
                                objTexCoords[vertIndices[1]] : Tex2f(0.f);
                            
            vertex.normal   = Vec3f(0.f);

            // Re-use vertex if possible, otherwise add new
            int meshVertIndex = mesh->getVertexIndex(vertex);
            if (meshVertIndex < 0)
            int    meshVertIndex = mesh->addVertex(vertex);

            mesh->addIndex(meshVertIndex, (v >= 3));
        }
    }

    // Finalize current mesh
    if (mesh)
    {
        mesh->setNormals();
        mesh->update();
    }

    t2.stopAndPrint();

    // Update bounding box
    update();

    // Debug
    debugAABB = new Cube(axisAlignedBox.getSize());
    debugAABBmaterial = new Material(Image::getSingleColor(Color(255, 255, 0, 20)));
}

void Base::Obj::cleanUp()
{
    for (TriangleMesh* mesh : meshes)
        delete mesh;

    for (Material* material : materials)
        delete material;
    
    meshes.clear();
    materials.clear();
}

void Base::Obj::loadMaterials(const TextFile* mtlFile)
{
    List<string> lines = stringGetLines(mtlFile->getText());
    Material* curMaterial = nullptr;

    for (const string& line : lines)
    {
        std::stringstream lineStream(line);
        string command;
        lineStream >> command;

        // Blankspace or comments
        if (command == "#" || command == "")
            continue;

        // New material
        else if (command == "newmtl")
        {
            string name = loadReadString(lineStream);
            curMaterial = new Material();
            mtlMap[name] = curMaterial;
        }

        // Only process remaining commands if a
        // material has been defined via "newmtl"
        else if (curMaterial)
        {
            // Ambient (Ka r g b)
            if (command == "Ka")
            {
                Color ambient;
                lineStream >> ambient.r >> ambient.g >> ambient.b;
                curMaterial->setAmbient(ambient);
            }
            
            // Diffuse (Kd r g b)
            else if (command == "Kd")
            {
                Color diffuse;
                lineStream >> diffuse.r >> diffuse.g >> diffuse.b;
                curMaterial->setDiffuse(diffuse);
            }
            
            // Specular (Ks r g b)
            else if (command == "Ks")
            {
                Color specular;
                lineStream >> specular.r >> specular.g >> specular.b;
                curMaterial->setSpecular(specular);
            }
            
            // Specular exponent (Ns f)
            else if (command == "Ns")
            {
                float specularExp;
                lineStream >> specularExp;
                curMaterial->setSpecularExp(specularExp);
            }
            
            // Transmission (Tf r g b)
            else if (command == "Tf")
            {
                Color transmission;
                lineStream >> transmission.r >> transmission.g >> transmission.b;
                curMaterial->setTransmission(transmission);
            }
            
            // Opacity (d f)
            else if (command == "d")
            {
                float opacity;
                lineStream >> opacity;
                curMaterial->setOpacity(opacity);
            }
            
            // Transparency (Tr f)
            else if (command == "Tr")
            {
                float transparency;
                lineStream >> transparency;
                curMaterial->setOpacity(1 - transparency);
            }

            // Ambient texture (map_Ka filename)
            else if (command == "map_Ka")
            {
                string name = loadReadString(lineStream);
                curMaterial->setAmbientTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Diffuse texture (map_Kd filename)
            else if (command == "map_Kd")
            {
                string name = loadReadString(lineStream);
                curMaterial->setDiffuseTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Specular texture (map_Ks filename)
            else if (command == "map_Ks")
            {
                string name = loadReadString(lineStream);
                curMaterial->setSpecularTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Specular exponent texture (map_Ns filename)
            else if (command == "map_Ns")
            {
                string name = loadReadString(lineStream);
                curMaterial->setSpecularExpTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Opacity texture (map_d filename)
            else if (command == "map_d")
            {
                string name = loadReadString(lineStream);
                curMaterial->setOpacityTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Bump texture (map_bump filename)
            else if (command == "map_bump")
            {
                curMaterial->setBumpTexture(
                    (Sprite*)appHandler->resHandler->get(getFilePath(name).getFullPath())
                );
            }

            // Illumination model (illum n)
            else if (command == "illum")
            {
                int illum;
                lineStream >> illum;
                curMaterial->setIllumModel((IlluminationModel)illum);
            }
        }
    }
}

Base::FilePath Base::Obj::getFilePath(const string& name)
{
    return FilePath(this->name).getDirectoryPath().getFilePath(name);
}

string Base::Obj::loadReadString(std::stringstream& lineStream)
{
    string str, word;
    str = "";
    while (!lineStream.eof())
    {
        lineStream >> word;
        str += (str != "") ? " " + word : word;
    }

    return str;
}