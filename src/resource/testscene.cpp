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
    JsonObject* jfMaterials = jf.getObject("materials");
    for (string name : jfMaterials->getKeys())
    {
        JsonObject* jfMat = jfMaterials->getObject(name);
        Image* img = (Image*)appHandler->resHandler->get(jfMat->getString("image"));
        sceneMaterialMap[name] = new Material(img);
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
            catch (ResourceException e) {} // Ignore missing resources
        }

        // Apply transforms
        if (jfObj->getKeyExists("transform"))
        {
            JsonArray* jfTransforms = jfObj->getArray("transform");
            for (JsonAny* jfTransformAny : jfTransforms->getValues())
            {
                JsonObject* jfTransform = (JsonObject*)jfTransformAny;
                string type = jfTransform->getString("type");
                Vec3f vec = jfTransform->getVec3<float>("vec");

                if (type == "scale")
                    obj->scale(vec);
                else if (type == "translate")
                    obj->translate(vec);
            }
        }

        // Finally add object to scene
        objects.add(obj);
    }
}

bool Base::TestScene::reload(const FilePath& file)
{
    // Cleanup
    for (Map<string, Material*>::iterator it = sceneMaterialMap.begin(); it != sceneMaterialMap.end(); it++)
        delete it->second;
    sceneMaterialMap.clear();

    for (TriangleMesh* mesh : meshes)
        delete mesh;
    meshes.clear();
    
    objects.clear();

    load(fileGetText(file));
    return true;
}