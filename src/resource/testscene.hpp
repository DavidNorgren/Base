#pragma once

#include "resource/resource.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"
#include "scene/scene.hpp"


namespace Base
{
    /* A JSON-generated scene, used for testing. */
    class TestScene : public Scene, public Resource
    {
      public:
        TestScene() {};

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(const string& json);
        bool reload(const FilePath& file) override;
        
        List<TriangleMesh*> meshes;
        Map<string, Material*> sceneMaterialMap;
    };
}