#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "common.hpp"
#include "resource/font.hpp"
#include "resource/resourcehandler.hpp"
#include "render/color.hpp"
#include "util/stringfunc.hpp"
#include "util/mathfunc.hpp"


constexpr uint FONTS_SIZE = 15;
constexpr uint FONTS_START = 32;
constexpr uint FONTS_END = 128;

EXPORT int Base::Font::stringGetWidth(const string& text)
{
    int width = 0, dx = 0;
    
    for (uint c = 0; c < text.size(); c++)
    {
        uchar curChar = text[c];

        if (curChar == '\n')
        {
            dx = 0;
            continue;
        }
        
        if (curChar < start || curChar > end)
            continue;
        
        CharInfo curCharInfo = chars[curChar];
        dx += curCharInfo.advance.x;
        
        width = max(width, dx);
    }
    
    return width;
}

EXPORT int Base::Font::stringGetHeight(const string& text)
{
    return (stringGetCount(text, "\n") + stringGetCount(text, "\r") + 1) * glTextureSize.height * LINE_SPACE;
}

void Base::Font::load(const FilePath& file)
{
    // Init library
    FT_Library lib;
    FT_Face face;
    FT_Init_FreeType(&lib);

    // Create the font map from a file on the disk
    if (FT_New_Face(lib, &file.getFullPath()[0], 0, &face))
        throw FontException("Could not open font " + file.getFullPath() + "!"); //TODO: can we get more info?
    
    load(face);
}

void Base::Font::load(const FileData& data)
{
    // Init library
    FT_Library lib;
    FT_Face face;
    FT_Init_FreeType(&lib);

    // Create the font map from a file in memory
    if (FT_New_Memory_Face(lib, (uchar*)&data[0], data.size(), 0, &face))
        throw FontException("Could not open font!"); //TODO: can we get more info?
    
    load(face);
}

void Base::Font::load(FT_Face& face)
{
    this->size  = FONTS_SIZE;
    this->start = FONTS_START;
    this->end   = FONTS_END;

    // Set size of the font
    FT_GlyphSlot glyph = face->glyph;
    FT_Set_Pixel_Sizes(face, 0, size);

    // Create character list
    chars = new CharInfo[end];

    // Get map dimensions
    glTextureSize = { 0, 0 };
    for (uint i = start; i < end; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        chars[i] = {
            { (int)glyph->bitmap.width, (int)glyph->bitmap.rows },
            { glyph->bitmap_left, glyph->bitmap_top },
            { (int)(glyph->advance.x / 64.f), (int)(glyph->advance.y / 64.f) },
            (float)glTextureSize.width,
        };

        glTextureSize.width += glyph->bitmap.width;
        glTextureSize.height = max(glTextureSize.height, (int)glyph->bitmap.rows);
    }

    // Create map texture
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glTextureSize.width, glTextureSize.height, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw bitmaps onto map
    for (uint i = start; i < end; i++)
    {
        CharInfo curChar = chars[i];

        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        // Convert buffer to RGBA
        Color* glyphBuf = new Color[(int)curChar.size.width * (int)curChar.size.height];
        for (uint j = 0; j < curChar.size.width * curChar.size.height; j++)
            glyphBuf[j] = Color(1.f, 1.f, 1.f, glyph->bitmap.buffer[j] / 255.f);

        glTexSubImage2D(GL_TEXTURE_2D, 0, curChar.mapX, 0, curChar.size.width, curChar.size.height, GL_RGBA, GL_FLOAT, glyphBuf);

        delete glyphBuf;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}