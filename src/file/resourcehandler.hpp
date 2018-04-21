#pragma once

#include "file/file.hpp"

#define FONTS_SIZE      15 // TODO: Make filename.ttf.ini with settings (and load after processing resources)
#define FONTS_START     32
#define FONTS_END       128


namespace Base
{
    class ResourceHandler
    {
      public:
        EXPORT ResourceHandler(void* data, size_t size);
        EXPORT File* find(string name);

      private:
        map<string, File*> resMap;
    };
}
