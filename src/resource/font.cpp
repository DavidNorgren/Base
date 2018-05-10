#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "common.hpp"
#include "resource/font.hpp"
#include "resource/resourcehandler.hpp"
#include "render/color.hpp"
#include "util/stringfunc.hpp"
#include "util/mathfunc.hpp"
#include "file/filefunc.hpp"


constexpr uint FONTS_SIZE    = 24;
constexpr uint FONTS_START   = 32;
constexpr uint FONTS_END     = 128;
constexpr uint BITMAP_WIDTH  = 512;
constexpr uint BITMAP_HEIGHT = 512;
constexpr float LINE_SPACE   = 1.f;

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
            charPos.y += charSize * LINE_SPACE;
            continue;
        }

        // Ignore characters out of range
        if (curChar < charStart || curChar > charEnd)
            continue;

        CharQuad cInfo = chars[curChar - charStart];

        // Only render visible characters
        if (cInfo.size.width && cInfo.size.height)
        {
            const Vec2i& p1 = charPos + cInfo.offset;
            const Vec2i& p2 = charPos + cInfo.offset + cInfo.size;
            const Tex2f& t1 = cInfo.texStart;
            const Tex2f& t2 = cInfo.texEnd;

            vertexData.add({ { p1.x, p1.y }, { t1.x, t1.y } });
            vertexData.add({ { p1.x, p2.y }, { t1.x, t2.y } });
            vertexData.add({ { p2.x, p1.y }, { t2.x, t1.y } });
            vertexData.add({ { p2.x, p1.y }, { t2.x, t1.y } });
            vertexData.add({ { p1.x, p2.y }, { t1.x, t2.y } });
            vertexData.add({ { p2.x, p2.y }, { t2.x, t2.y } });
            vertices += 6;
        }

        charPos += cInfo.advance;
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
        
        CharQuad curCharInfo = chars[curChar - charStart];
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
    load(fileGetData(file));
}

void Base::Font::load(const FileData& data)
{
    charSize  = FONTS_SIZE;
    charStart = FONTS_START;
    charEnd   = FONTS_END;
    size      = { BITMAP_WIDTH, BITMAP_HEIGHT };

    stbtt_pack_context pc;
    stbtt_packedchar* packedChar = new stbtt_packedchar[charEnd - charStart];
    uchar* tempBitmap = new uchar[size.width * size.height];
    Color* pixelData = new Color[size.width * size.height];

    // Get info about font
    stbtt_fontinfo info;
    int baseline, ascent, descent, linegap;
    int ret = stbtt_InitFont(&info, (const uchar*)&data[0], stbtt_GetFontOffsetForIndex((const uchar*)&data[0], 0));
    float scale = stbtt_ScaleForPixelHeight(&info, charSize);
    stbtt_GetFontVMetrics(&info, &ascent,&descent,&linegap);
    baseline = (int) (ascent * scale);

    // Pack font onto atlas
    stbtt_PackBegin(&pc, tempBitmap, size.width, size.height, 0, 1, nullptr);
    stbtt_PackSetOversampling(&pc, 1, 1);
    stbtt_PackFontRange(&pc, (const uchar*)&data[0], 0, charSize, charStart, charEnd - charStart, packedChar);
    stbtt_PackEnd(&pc);

    // Parse characters
    for (int c = charStart; c < charEnd; c++)
    {
        stbtt_aligned_quad quad;
        float x, y;
        x = y = 0.f;
        stbtt_GetPackedQuad(packedChar, size.width, size.height, c - charStart, &x, &y, &quad, true);

        CharQuad cInfo;
        cInfo.offset   = { (int)quad.x0, baseline + (int)quad.y0 };
        cInfo.size     = { (int)(quad.x1 - quad.x0), (int)(quad.y1 - quad.y0) };
        cInfo.texStart = { quad.s0, quad.t0 };
        cInfo.texEnd   = { quad.s1, quad.t1 };
        cInfo.advance  = { (int)x, 0 };
        chars.add(cInfo);
    }

    // Convert from 1 channel alpha to RGBA for the texture
    for (uint x = 0; x < size.width; x++)
        for (uint y = 0; y < size.height; y++)
            pixelData[x + y * size.width] = Color(1.f, 1.f, 1.f, tempBitmap[x + y * size.width] / 255.f);

    // Create atlas texture
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_FLOAT, pixelData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up
    delete packedChar;
    delete tempBitmap;
    delete pixelData;
}

void Base::Font::cleanUp()
{
    glDeleteTextures(1, &glTexture);  
}