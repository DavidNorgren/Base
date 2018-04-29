
#include "tiny_obj_loader.hpp"

#include "common.hpp"
#include "resource/model.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"
#include "util/timer.hpp"


void Base::Model::render(Shader* shader, const Mat4f& projMat) const
{
    for (TriangleMesh* mesh : meshes)
        mesh->render(shader, projMat * matrix);
}

void Base::Model::load(const FilePath& file)
{
    List<string> lines = fileGetLines(file);
    load(lines);
}

void Base::Model::load(const FileData& data)
{
    List<string> lines = stringGetLines(string(&data[0], data.size()));
    load(lines);
}

void Base::Model::load(const List<string>& lines)
{
    List<Vec3f> objPositions, objNormals;
    List<Vec2f> objTexCoords;
    List<Vec3i> objIndices;
    List<uint> objFaceNumVertices;

    // todo OMP
    Timer t1("Obj: Parse");
    for (const string& line : lines)
    {
		std::stringstream lineStream(line);
        string type;
        lineStream >> type;

         // Position (v x y z [w])
		if (type == "v")
        {
			Vec3f pos;
			lineStream >> pos.x >> pos.y >> pos.z;
			objPositions.add(pos);
		}

         // Normal (vn x y z)
        else if (type == "vn")
        {
			Vec3f norm;
			lineStream >> norm.x >> norm.y >> norm.z;
			objNormals.add(norm);
		}

        // Texture coordinate (vt u v [w])
        else if (type == "vt")
        {
			Vec2f texCoord;
			lineStream >> texCoord.x >> texCoord.y;
			objTexCoords.add(texCoord);
		}

        // Index (f v1 v2 v3 ... vN for a N-sided polygon)
        // The indices are 1-based for now, with 0 representing left-out values
        else if (type == "f")
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

    Timer t2("Obj: Generate data");
    TriangleMesh* mesh = new TriangleMesh(new Material()); // TODO material from mtlib

    // Go through each face
    uint index = 0;
    for (uint num : objFaceNumVertices)
    {
        uint faceStartIndex, faceLastIndex;
        for (uint v = 0; v < num; v++, index++)
        {
            Vec3i vIndices = objIndices[index];
            Vertex3Df vertex;

            // Set texture/normal index to position index if unset
            if (vIndices[1] == 0)
                vIndices[1] = vIndices[0];

            if (vIndices[2] == 0)
                vIndices[2] = vIndices[0];

            // Make zero-based and find values
            vIndices[0]--; vIndices[1]--; vIndices[2]--;

            vertex.pos      = (vIndices[0] < objPositions.size()) ? 
                                objPositions[vIndices[0]] : Vec3f(0.f);
                
            vertex.texCoord = (vIndices[1] < objTexCoords.size()) ? 
                                objTexCoords[vIndices[1]] : Vec2f(0.f);
                            
            vertex.normal   = Vec3f(0.f);

            // Re-use vertex if possible, otherwise add new
            int vIndex = mesh->getVertexIndex(vertex);
            if (vIndex < 0)
                vIndex = mesh->addVertex(vertex);

            // Generate triangles by adding references to the first
            // and last added vertex in this polygon
            if (v == 0)
                faceStartIndex = vIndex;
            else if (v >= 3)
            {
                mesh->addIndex(faceStartIndex);
                mesh->addIndex(faceLastIndex);
            }

            mesh->addIndex(vIndex);
            faceLastIndex = vIndex;
        }
    }

    t2.stopAndPrint();

    mesh->setNormals();
    mesh->update();
    meshes.add(mesh);
}

void Base::Model::cleanUp()
{
    for (TriangleMesh* mesh : meshes)
        delete mesh;

    for (Material* material : materials)
        delete material;
    
    meshes.clear();
    materials.clear();
}