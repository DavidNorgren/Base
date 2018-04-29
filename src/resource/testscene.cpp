#include "common.hpp"
#include "resource/testscene.hpp"
#include "resource/model.hpp"
#include "file/filefunc.hpp"
#include "file/json.hpp"
#include "apphandler.hpp"


void Base::TestScene::load(const FilePath& file)
{
    load(fileGetText(file));
}

void Base::TestScene::load(const FileData& data)
{
    load(string(&data[0], data.size()));
}

void Base::TestScene::load(const string& json)
{
    JsonFile jf(json);

    // Background color
    appHandler->mainWindow->setBackgroundColor(jf.getString("backgroundColor"));

    // Define materials
    if (jf.getKeyExists("materials"))
    {
        JsonObject* jfMaterials = jf.getObject("materials");
        for (string name : jfMaterials->getKeys())
        {
            JsonObject* jfMat = jfMaterials->getObject(name);
            Image* img = (Image*)appHandler->resHandler->get(jfMat->getString("image"));
            sceneMaterialMap[name] = new Material(img);
        }
    }

    // Objects
    JsonArray* jfObjects = jf.getArray("objects");
    for (JsonAny* jfAny : jfObjects->getValues())
    {
        JsonObject* jfObj = (JsonObject*)jfAny;
        Object* obj;

        // Find material from name
        string model = jfObj->getString("model");
        Material* material;
        if (jfObj->getKeyExists("material"))
            material = sceneMaterialMap[jfObj->getString("material")];

        // Plane
        if (model == "plane")
        {
            Size2Df size = jfObj->getSize2D<float>("size");
            Vec2f repeat = jfObj->getVec2<float>("repeat");
            obj = new Plane(size, material, repeat);
            meshes.add((TriangleMesh*)obj);
        }

        // Model
        else
        {
            try
            {
                obj = (Model*)appHandler->resHandler->get(model);
                obj->resetTransform();
            }
            catch (ResourceException e)
            {
                continue; // Ignore missing resources
            }
        }

        // Apply transforms
        if (jfObj->getKeyExists("position"))
            obj->translate(jfObj->getVec3<float>("position"));
            
        if (jfObj->getKeyExists("rotation"))
            obj->rotate(jfObj->getVec3<float>("rotation"));
            
        if (jfObj->getKeyExists("scale"))
            obj->scale(jfObj->getVec3<float>("scale"));
            
        obj->buildMatrix();

        // Finally add object to scene
        objects.add(obj);
    }
}

void Base::TestScene::cleanUp()
{
    // Cleanup
    for (Map<string, Material*>::iterator it = sceneMaterialMap.begin(); it != sceneMaterialMap.end(); it++)
        delete it->second;

    for (TriangleMesh* mesh : meshes)
        delete mesh;

    sceneMaterialMap.clear();
    meshes.clear();
    objects.clear();
}