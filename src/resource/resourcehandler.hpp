#pragma once

#include "resource/resource.hpp"


namespace Base
{
    struct ResourceException : public runtime_error
    {
        ResourceException(const string& message) : runtime_error(message) {};
    };

    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler();
        EXPORT Resource* get(const string& name);

      private:
        Map<string, Resource*> resMap;
    };
}
