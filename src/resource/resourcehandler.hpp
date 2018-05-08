#pragma once

#include "resource/resource.hpp"


namespace Base
{
    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler(char resData[], uint resSize);
        EXPORT ResourceHandler(const DirectoryPath& dynamicDir);
        EXPORT Resource* get(const string& name);

      private:
        Map<string, Resource*> resMap;
        bool dynamicResources;
        DirectoryPath dynamicDir;
    };
}
