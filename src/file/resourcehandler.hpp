#pragma once

#include "file/file.hpp"


namespace Base
{
    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler(void* data, uint size);
        EXPORT File* find(string name);

      private:
        Map<string, File*> resMap;
    };
}
