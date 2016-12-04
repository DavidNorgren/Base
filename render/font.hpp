#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "render/color.hpp"
#include "util/math/mathfunc.hpp"
#include "util/stringfunc.hpp"
#include "file/resourcehandler.hpp"

#define LINE_SPACE  1.25


namespace Base
{
    
    struct CharInfo
    {
        float width, height, left, top, advanceX, advanceY, mapX;
    };

    class Font {

        public:

            /* Load a new font from a file. */
            Font(string filename, uint size, uint start, uint end);
            Font(File* file,      uint size, uint start, uint end);

            /* Get text dimensions. */
            int stringGetWidth(string text);
            int stringGetHeight(string text);

            uint start, end, size;
            int width, height;
            GLuint texture;
            CharInfo* chars;

        private:

            void load(FT_Face& face);

    };
    
}
