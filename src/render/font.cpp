#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "common.hpp"
#include "render/font.hpp"
#include "render/color.hpp"
#include "file/resourcehandler.hpp"
#include "util/stringfunc.hpp"
#include "util/mathfunc.hpp"


Base::Font::Font(string filename, uint start, uint end, uint size)
{
    // Init library
    FT_Library lib;
    FT_Face face;
    FT_Init_FreeType(&lib);

    this->start = start;
    this->end = end;
    this->size = size;

    cout << "Loading " << filename << "..." << endl;

    // Create the font map from a file on the disk
    if (FT_New_Face(lib, &filename[0], 0, &face))
    {
        cout << "Could not open font!" << endl;
        return;
    }
    
    load(face);
}

Base::Font::Font(File* file, uint size, uint start, uint end)
{
    // Init library
    FT_Library lib;
    FT_Face face;
    FT_Init_FreeType(&lib);

    this->size = size;
    this->start = start;
    this->end = end;

    cout << "Loading " << file->name << "..." << endl;

    // Create the font map from a file in memory
    if (FT_New_Memory_Face(lib, (uchar*)file->rawData, file->size, 0, &face))
    {
        cout << "Could not open font!" << endl;
        return;
    }
    
    load(face);
}

void Base::Font::load(FT_Face& face)
{
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

int Base::Font::stringGetWidth(string text)
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

int Base::Font::stringGetHeight(string text)
{
    return (stringGetCount(text, "\n") + stringGetCount(text, "\r") + 1) * glTextureSize.height * LINE_SPACE;
}