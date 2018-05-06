#pragma once

#include "resource/resource.hpp"


namespace Base
{
    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler();
        EXPORT Resource* get(const string& name);

      private:
        Map<string, Resource*> resMap;
    };
}
