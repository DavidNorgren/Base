#pragma once

#include "resource/resource.hpp"


namespace Base
{
    struct ResourceException
    {
        ResourceException(const string& message) : message(message) {};
        const string& what() const { return message; }

      private:
        string message;
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
