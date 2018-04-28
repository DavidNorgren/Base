#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "resource/resource.hpp"
#include "util/data/size2d.hpp"
#include "util/data/vec2.hpp"


namespace Base
{
    constexpr float LINE_SPACE = 1.25f;

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
        Font() {};

        /* Get text dimensions. */
        EXPORT int stringGetWidth(const string& text);
        EXPORT int stringGetHeight(const string& text);

        uint start, end, size;
        CharInfo* chars;

        GLuint glTexture;
        Size2Di glTextureSize;

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(FT_Face& face);
    };
}
