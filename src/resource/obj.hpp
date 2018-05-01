#pragma once

#include "util/data/list.hpp"
#include "resource/resource.hpp"

#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"
#include "scene/model.hpp"


namespace Base
{
    class Obj : public Model, public Resource
    {
      public:
        Obj() {};

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(const List<string>& lines);
        void cleanUp() override;
    };
}