#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "file/file.hpp"

#define LINE_SPACE  1.25f


namespace Base
{
    struct CharInfo
    {
        float width, height, left, top, advanceX, advanceY, mapX;
    };

    class Font
    {
      public:
        /* Load a new font from a file. */
        Font(string filename, uint size, uint start, uint end);
        Font(File* file,      uint size, uint start, uint end);

        /* Get text dimensions. */
        int stringGetWidth(string text);
        int stringGetHeight(string text);

        uint start, end, size;
        int width, height;
        uint texture;
        CharInfo* chars;

      private:
        void load(FT_Face& face);
    };
}
