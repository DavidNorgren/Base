#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "file/resource.hpp"
#include "util/data/size2d.hpp"
#include "util/data/vec2.hpp"

namespace Base
{
    constexpr float LINE_SPACE = 1.25f;
    constexpr uint FONTS_SIZE = 15;
    constexpr uint FONTS_START = 32;
    constexpr uint FONTS_END = 128;

    struct FontException : public runtime_error
    {
        FontException(string message) : runtime_error(message) {};
    };

    enum class FontStyle
    {
        NORMAL,
        BOLD
    };
    
    struct CharInfo
    {
        Size2Di size;
        Vec2i pos, advance;
        float mapX;
    };

    class Font : public Resource
    {
      public:
        /* Load a new font from a file. */
        EXPORT Font(string filename,  uint size = FONTS_SIZE, uint start = FONTS_START, uint end = FONTS_END);
        EXPORT Font(const Data& data, uint size = FONTS_SIZE, uint start = FONTS_START, uint end = FONTS_END);

        /* Get text dimensions. */
        EXPORT int stringGetWidth(string text);
        EXPORT int stringGetHeight(string text);

        uint start, end, size;
        CharInfo* chars;

        GLuint glTexture;
        Size2Di glTextureSize;

      private:
        void load(FT_Face& face);
    };
}
