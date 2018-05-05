#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "resource/resource.hpp"
#include "render/image.hpp"
#include "util/data/list.hpp"
#include "util/data/size2d.hpp"
#include "util/data/vec2.hpp"
#include "util/data/vertex2d.hpp"


namespace Base
{
    constexpr float LINE_SPACE = 1.25f;

    struct FontException : public ResourceLoadException
    {
        FontException(string message) : ResourceLoadException(message) {};
    };
    
    /* Info about a single character in a font. */
    struct CharInfo
    {
        Size2Di size;
        Vec2i pos, advance;
        float mapX;
    };

    /* A texture sheet of characters and methods for calculating
       string dimensions. */
    class Font : public Image, public Resource
    {
      public:
        Font() {};

        /* Get a list of vertices to render for the given text. */
        EXPORT List<Vertex2Di> getTextVertices(const string& text);

        /* Get text dimensions. */
        EXPORT int getTextWidth(const string& text);
        EXPORT int getTextHeight(const string& text);

      private:
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        void load(FT_Face& face);
        void cleanUp() override;
        
        uint charSize, charStart, charEnd;
        List<CharInfo> chars;
    };
}
