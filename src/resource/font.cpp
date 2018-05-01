#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "common.hpp"
#include "resource/font.hpp"
#include "resource/resourcehandler.hpp"
#include "render/color.hpp"
#include "util/stringfunc.hpp"
#include "util/mathfunc.hpp"


constexpr uint FONTS_SIZE  = 15;
constexpr uint FONTS_START = 32;
constexpr uint FONTS_END   = 128;

EXPORT Base::List<Base::Vertex2Di> Base::Font::getTextVertices(const string& text)
{
    List<Vertex2Di> vertexData(text.length() * 6);
    ScreenPos charPos = { 0, 0 };
    int vertices = 0;
    
    for (uint c = 0; c < text.length(); c++)
    {
        uchar curChar = text[c];

        // Skip to next line
        if (curChar == '\n' || curChar == '\r')
        {
            charPos.x = 0;
            charPos.y += size.height * LINE_SPACE;
            continue;
        }

        // Ignore characters out of range
        if (curChar < charStart || curChar > charEnd)
            continue;

        CharInfo curCharInfo = chars[curChar];

        // Only render visible characters
        if (curCharInfo.size.width && curCharInfo.size.height)
        {
            int vx   = charPos.x + curCharInfo.pos.x;
            int vy   = charPos.y + size.height - curCharInfo.pos.y;
            int vw   = curCharInfo.size.width;
            int vh   = curCharInfo.size.height;
            float tx = curCharInfo.mapX / size.width;
            float tw = (float)curCharInfo.size.width / size.width;
            float th = (float)curCharInfo.size.height / size.height;

            vertexData.add({ { vx, vy },           { tx, 0.f } });
            vertexData.add({ { vx, vy + vh },      { tx, th } });
            vertexData.add({ { vx + vw, vy },      { tx + tw, 0.f } });
            vertexData.add({ { vx + vw, vy },      { tx + tw, 0.f } });
            vertexData.add({ { vx, vy + vh },      { tx, th } });
            vertexData.add({ { vx + vw, vy + vh }, { tx + tw, th } });
            vertices += 6;
        }

        charPos += curCharInfo.advance;
    }

    vertexData.resize(vertices);
    return vertexData;
}

EXPORT int Base::Font::getTextWidth(const string& text)
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
        
        if (curChar < charStart || curChar > charEnd)
            continue;
        
        CharInfo curCharInfo = chars[curChar];
        dx += curCharInfo.advance.x;
        
        width = max(width, dx);
    }
    
    return width;
}

EXPORT int Base::Font::getTextHeight(const string& text)
{
    return (stringGetCount(text, "\n") + stringGetCount(text, "\r") + 1) * size.height * LINE_SPACE;
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
    this->charSize  = FONTS_SIZE;
    this->charStart = FONTS_START;
    this->charEnd   = FONTS_END;

    // Set size of the font
    FT_GlyphSlot glyph = face->glyph;
    FT_Set_Pixel_Sizes(face, 0, charSize);

    // Create character list
    chars.reserve(charEnd);

    // Get map dimensions
    size = { 0, 0 };
    for (uint i = charStart; i < charEnd; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        chars[i] = {
            { (int)glyph->bitmap.width, (int)glyph->bitmap.rows },
            { glyph->bitmap_left, glyph->bitmap_top },
            { (int)(glyph->advance.x / 64.f), (int)(glyph->advance.y / 64.f) },
            (float)size.width,
        };

        size.width += glyph->bitmap.width;
        size.height = max(size.height, (int)glyph->bitmap.rows);
    }

    // Create map texture
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw bitmaps onto map
    for (uint i = charStart; i < charEnd; i++)
    {
        CharInfo curChar = chars[i];

        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        // Convert buffer to RGBA
        Color* glyphBuf = new Color[curChar.size.x * curChar.size.y];
        for (uint x = 0; x < curChar.size.x; x++)
            for (uint y = 0; y < curChar.size.y; y++)
                glyphBuf[x + y * curChar.size.x] = Color(1.f, 1.f, 1.f, glyph->bitmap.buffer[x + (curChar.size.y - 1 - y) * curChar.size.x] / 255.f);

        glTexSubImage2D(GL_TEXTURE_2D, 0, curChar.mapX, size.height - curChar.size.height, curChar.size.width, curChar.size.height, GL_RGBA, GL_FLOAT, glyphBuf);

        delete glyphBuf;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Base::Font::cleanUp()
{
    glDeleteTextures(1, &glTexture);  
}