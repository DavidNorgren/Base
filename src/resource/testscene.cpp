#include "common.hpp"
#include "resource/testscene.hpp"
#include "file/filefunc.hpp"
#include "file/json.hpp"
#include "scene/model.hpp"
#include "resource/obj.hpp"
#include "resource/sprite.hpp"
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
    try
    {
        JsonFile jf(json);

        // Background color
        background = jf.getString("backgroundColor");

        // Define materials
        if (jf.getKeyExists("materials"))
        {
            JsonObject* jfMaterials = jf.getObject("materials");
            for (string name : jfMaterials->getKeys())
            {
                JsonObject* jfMat = jfMaterials->getObject(name);
                Sprite* img = (Sprite*)appHandler->resHandler->get(jfMat->getString("image"));
                sceneMaterialMap[name] = new Material(img);
            }
        }

        // Objects
        JsonArray* jfObjects = jf.getArray("objects");
        for (JsonAny* jfAny : jfObjects->getValues())
        {
            JsonObject* jfObj = (JsonObject*)jfAny;
            Model* model = nullptr;

            // Find material from name
            string modelName = jfObj->getString("model");
            Material* material = nullptr;
            if (jfObj->getKeyExists("material"))
                material = sceneMaterialMap[jfObj->getString("material")];

            // Cube
            if (modelName == "cube")
            {
                Size3Df size = jfObj->getSize3D<float>("size");
                Vec2f repeat = { 1.f };
                if (jfObj->getKeyExists("repeat"))
                    repeat = jfObj->getVec2<float>("repeat");
                
                TriangleMesh* cube = new Cube(size, repeat);
                model = new Model(cube, material);
                meshes.add(cube);
            }
            
            // Plane
            else if (modelName == "plane")
            {
                Size2Df size = jfObj->getSize2D<float>("size");
                Vec2f repeat = { 1.f };
                if (jfObj->getKeyExists("repeat"))
                    repeat = jfObj->getVec2<float>("repeat");
                
                TriangleMesh* plane = new Plane(size, repeat);
                model = new Model(plane, material);
                meshes.add(plane);
            }

            // Model
            else
            {
                try
                {
                    model = (Obj*)appHandler->resHandler->get(modelName);
                }
                catch (const ResourceException& e)
                {
                    cout << "[TestScene] ResourceException: " << e.what() << endl;
                    continue; // Ignore missing resources
                }
            }

            // Add object
            string objName = "";
            if (jfObj->getKeyExists("name"))
                objName = jfObj->getString("name");

            Object* obj = new Object(model, objName);

            // Apply transforms
            if (jfObj->getKeyExists("position"))
                obj->translate(jfObj->getVec3<float>("position"));
                
            if (jfObj->getKeyExists("rotation"))
                obj->rotate(jfObj->getVec3<float>("rotation"));
                
            if (jfObj->getKeyExists("scale"))
                obj->scale(jfObj->getVec3<float>("scale"));
                
            // Toggle occlude
            if (jfObj->getKeyExists("occlude"))
                obj->setOcclude(jfObj->getBool("occlude"));
            
            obj->buildMatrix();

            objects.add(obj);
            if (objName != "")
                objectNames[objName] = obj;
        }

        // Lights
        if (jf.getKeyExists("lights"))
        {
            JsonArray* jfLights = jf.getArray("lights");
            for (JsonAny* jfAny : jfLights->getValues())
            {
                JsonObject* jfLight = (JsonObject*)jfAny;
                Light* light = new Light();
                light->setPosition(jfLight->getVec3<float>("position"));
                light->setColor(jfLight->getString("color"));
                // TODO type

                lights.add(light);
            }
        }
    }
    catch (const JsonException& jsEx)
    {
        throw TestSceneException(jsEx.what());
    }
}

void Base::TestScene::cleanUp()
{
    // Cleanup
    for (Map<string, Material*>::iterator it = sceneMaterialMap.begin(); it != sceneMaterialMap.end(); it++)
        delete it->second;

    for (TriangleMesh* mesh : meshes)
        delete mesh;
        
    for (Light* light : lights)
        delete light;

    sceneMaterialMap.clear();
    meshes.clear();
    objects.clear();
    lights.clear();
}