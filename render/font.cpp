#include "render/font.hpp"


Base::Font::Font(string filename, uint start, uint end, uint size)
{
    // Init library
    FT_Library lib;
    FT_Face face;
    FT_Init_FreeType(&lib);

    this->start = start;
    this->end = end;
    this->size = size;

    std::cout << "Loading " << filename << "..." << std::endl;

    // Create the font map from a file on the disk
    if (FT_New_Face(lib, &filename[0], 0, &face)) {
        std::cout << "Could not open font!" << std::endl;
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

    std::cout << "Loading " << file->name << "..." << std::endl;

    // Create the font map from a file in memory
    if (FT_New_Memory_Face(lib, (uchar*)file->rawData, file->size, 0, &face)) {
        std::cout << "Could not open font!" << std::endl;
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
    width = 0;
    height = 0;
    for (uint i = start; i < end; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            continue;
        }

        chars[i] = {
            (float)glyph->bitmap.width,
            (float)glyph->bitmap.rows,
            (float)glyph->bitmap_left,
            (float)glyph->bitmap_top,
            (float)glyph->advance.x / 64,
            (float)glyph->advance.y / 64,
            (float)width,
        };

        width += glyph->bitmap.width;
        height = max(height, (uint)glyph->bitmap.rows);
    }

    // Create map texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw bitmaps onto map
    for (uint i = start; i < end; i++)
    {
        CharInfo curChar = chars[i];

        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            continue;
        }

        // Convert buffer to RGBA
        Color* glyphBuf = new Color[(int)curChar.width * (int)curChar.height];
        for (uint j = 0; j < curChar.width * curChar.height; j++) {
            glyphBuf[j] = Color(1.f, 1.f, 1.f, glyph->bitmap.buffer[j] / 255.f);
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, curChar.mapX, 0, curChar.width, curChar.height, GL_RGBA, GL_FLOAT, glyphBuf);

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

        if (curChar == '\n') {
            dx = 0;
            continue;
        }
        
        if (curChar < start || curChar > end) {
            continue;
        }
        
        CharInfo curCharInfo = chars[curChar];
        dx += curCharInfo.advanceX;
        
        width = max(width, dx);
    }
    
    return width;
}


int Base::Font::stringGetHeight(string text)
{
    return (stringGetCount(text, "\n") + 1) * height * LINE_SPACE;
}