#pragma once

#include "file/resource.hpp"


namespace Base
{
    struct ResourceException : public runtime_error
    {
        ResourceException(string message) : runtime_error(message) {};
    };

    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler();
        EXPORT Resource* get(string name);

        void checkReload();

      private:
        void load();
        Map<string, Resource*> resMap;
        uint zipLastModified = 0;
    };
}
