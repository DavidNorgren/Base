#pragma once

#include <map>

#include "file/filefunc.hpp"
#include "file/zipfunc.hpp"

#define FONTS_EXT       L".ttf"
#define FONTS_SIZE      15 // TODO: Make filename.ttf.ini with settings (and load after processing resources)
#define FONTS_START     32
#define FONTS_END       128
#define SHADERS_EXT     L".glsl"
#define IMAGES_EXT      L".png"


namespace Base
{
    
    class ResourceHandler
    {
        public:

            ResourceHandler();
            File* find(string name);

        private:

            std::map<string, File*> resMap;
    };

}
