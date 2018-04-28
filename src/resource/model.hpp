#pragma once

#include "util/data/list.hpp"
#include "resource/resource.hpp"
#include "scene/object.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"


namespace Base
{
    struct ModelFileException : public runtime_error
    {
        ModelFileException(const string& message) : runtime_error(message) {};
    };

    class Model : public Object, public Resource
    {
      public:
        Model() {};

        void render(Shader* shader, const Mat4f& projMat) const override;

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(const List<string>& lines);
        bool reload(const FilePath& file) override;

        List<TriangleMesh*> meshes;
        List<Material*> materials;
    };
}