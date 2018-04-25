#include "common.hpp"
#include "apphandler.hpp"
#include "render/drawfunc.hpp"
#include "util/stringfunc.hpp"


EXPORT void Base::drawBegin()
{
    appHandler->drawingShader->select();
    resetDrawingArea();
}

EXPORT void Base::drawClear(Color color)
{
    glClearColor(color.r, color.g, color.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

EXPORT void Base::setDrawingArea(ScreenArea area)
{
    glEnable(GL_SCISSOR_TEST);
    appHandler->drawingArea = area;
    glScissor(area.x, appHandler->mainWindow->size.height - (area.y + area.height), area.width, area.height);
}

EXPORT Base::ScreenArea Base::getDrawingArea()
{
    return appHandler->drawingArea;
}

EXPORT void Base::resetDrawingArea()
{
    appHandler->drawingArea = { 0, 0, appHandler->mainWindow->size.width, appHandler->mainWindow->size.height };
    glDisable(GL_SCISSOR_TEST);
}

EXPORT void Base::setDrawingAlpha(float alpha)
{
    appHandler->drawingAlpha = alpha;
}

EXPORT float Base::getDrawingAlpha()
{
    return appHandler->drawingAlpha;
}

EXPORT void Base::drawText(string text, ScreenPos pos, Color color)
{
    drawText(text, pos, appHandler->drawingFont, color);
}

EXPORT void Base::drawText(string text, ScreenPos pos, Font* font, Color color)
{
    Vertex2Di vertexData[text.length() * 6];
    ScreenPos charPos = { 0, 0 };
    int vertexNum = 0;
    
    for (uint c = 0; c < text.length(); c++)
    {
        uchar curChar = text[c];

        // Skip to next line
        if (curChar == '\n' || curChar == '\r')
        {
            charPos.x = 0;
            charPos.y += font->glTextureSize.height * LINE_SPACE;
            continue;
        }

        // Ignore characters out of range
        if (curChar < font->start || curChar > font->end)
            continue;

        CharInfo curCharInfo = font->chars[curChar];

        // Only render visible characters
        if (curCharInfo.size.width && curCharInfo.size.height)
        {
            int vx = charPos.x + curCharInfo.pos.x;
            int vy = charPos.y + font->glTextureSize.height - curCharInfo.pos.y;
            int vw = curCharInfo.size.width;
            int vh = curCharInfo.size.height;
            float tx = curCharInfo.mapX / font->glTextureSize.width;
            float tw = (float)curCharInfo.size.width / font->glTextureSize.width;
            float th = (float)curCharInfo.size.height / font->glTextureSize.height;

            vertexData[vertexNum++] = { { vx, vy },           { tx, 0.f } };
            vertexData[vertexNum++] = { { vx, vy + vh },      { tx, th } };
            vertexData[vertexNum++] = { { vx + vw, vy },      { tx + tw, 0.f } };
            vertexData[vertexNum++] = { { vx + vw, vy },      { tx + tw, 0.f } };
            vertexData[vertexNum++] = { { vx, vy + vh },      { tx, th } };
            vertexData[vertexNum++] = { { vx + vw, vy + vh }, { tx + tw, th } };
        }

        charPos += curCharInfo.advance;
    }

    Mat4f mat = appHandler->mainWindow->ortho * Mat4f::translate({ (float)pos.x, (float)pos.y, 0 });
                 
    appHandler->drawingShader->render2D(mat, vertexData, vertexNum,
                                        font->glTexture, Color(color, appHandler->drawingAlpha));
}

EXPORT void Base::drawTextAligned(string text, ScreenPos pos, TextAlignX alignX, TextAlignY alignY, Color color)
{
    drawTextAligned(text, pos, appHandler->drawingFont, alignX, alignY, color);
}

EXPORT void Base::drawTextAligned(string text, ScreenPos pos, Font* font, TextAlignX alignX, TextAlignY alignY, Color color)
{
    // Vertical alignment
    if (alignY != TextAlignY::TOP)
    {
        int height = font->stringGetHeight(text);
        
        if (alignY == TextAlignY::MIDDLE)
            pos.y -= height / 2;

        else if (alignY == TextAlignY::BOTTOM)
            pos.y -= height;
    }
        
    // Horizontal alignment
    if (alignX == TextAlignX::LEFT)
        drawText(text, pos, font, color);
    else
    {
        for (string line : stringSplit(text, "\n"))
        {
            int width = font->stringGetWidth(line);
            
            if (alignX == TextAlignX::CENTER)
                pos.x -= width / 2;

            else if (alignX == TextAlignX::RIGHT)
                pos.x -= width;
            
            drawText(line, pos, font, color);
        }
    }
}

EXPORT void Base::drawImage(string name, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    drawImage((Image*)appHandler->resHandler->get(name), pos, color, rotation, scale);
}

EXPORT void Base::drawImage(Image* image, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    Size2Di size = image->glTextureSize;
    Vertex2Di vertexData[4] = {
        { { 0, 0 },                    { 0, 0 } },
        { { size.width, 0 },           { 1, 0 } },
        { { 0, size.height },          { 0, 1 } },
        { { size.width, size.height }, { 1, 1 } }
    };
    
    Mat4f mat = appHandler->mainWindow->ortho *
                    Mat4f::translate({ (float)pos.x, (float)pos.y, 0 }) *
                    Mat4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                    Mat4f::scale({ scale.x, scale.y, 1.f });

    appHandler->drawingShader->render2D(mat, vertexData, 4,
                                        image->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawSubImage(string name, int subImage, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    drawSubImage((Image*)appHandler->resHandler->get(name), subImage, pos, color, rotation, scale);
}

EXPORT void Base::drawSubImage(Image* image, int subImage, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    Size2Di size = image->glTextureSize;
    int subImages = size.width / size.height;
    Vec2f texStart = { (float)subImage / (float)subImages, 0.f };
    Vec2f texEnd = { texStart.x + (float)size.height / size.width, 1.f };
    
    Vertex2Di vertexData[4] = {
        { { 0, 0 },                     texStart },
        { { size.height, 0 },           { texEnd.x, texStart.y } },
        { { 0, size.height },           { texStart.x, texEnd.y } },
        { { size.height, size.height }, texEnd }
    };
    
    Mat4f mat = appHandler->mainWindow->ortho *
                Mat4f::translate({ (float)pos.x, (float)pos.y, 0 }) *
                Mat4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                Mat4f::scale({ scale.x, scale.y, 1.f });
                 
    appHandler->drawingShader->render2D(mat, vertexData, 4, 
                                        image->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawBox(ScreenArea box, Color color, bool outline, int outlineThickness)
{
    Vertex2Di vertexData[5] = {
        { { box.x, box.y },                          { 0, 0 } },
        { { box.x + box.width, box.y },              { 0, 0 } },
        { { box.x + box.width, box.y + box.height }, { 0, 0 } },
        { { box.x, box.y + box.height },             { 0, 0 } },
        { { box.x, box.y },                          { 0, 0 } }
    };

    glLineWidth(outlineThickness);
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, vertexData, 5,
                                        appHandler->solidColor->glTexture, Color(color, appHandler->drawingAlpha),
                                        outline ? GL_LINE_STRIP : GL_TRIANGLE_FAN);
}

EXPORT void Base::drawBoxEdges(ScreenArea box, Color color, string edgeImage, bool edgeTopLeft, bool edgeTopRight, bool edgeBottomRight, bool edgeBottomLeft)
{
    /*Image* edge = (Image*)appHandler->resHandler->find(edgeImage)->loaded;
    int edgeWidth = edge->glTextureSize.width;
    int edgeHeight = edge->glTextureSize.height;
    int i = 0;
    int numVertex = 2 +
                    (1 + 2 * edgeTopRight) +
                    (1 + 2 * edgeTopLeft) +
                    (1 + 2 * edgeBottomLeft) +
                    (1 + 2 * edgeBottomRight);
    
    ScreenPos& pos = box.pos;
    Vec3f posData[numVertex];
    Vec2f texCoordData[numVertex];
    
    // Center vertex
    posData[i++] = { pos.x + box.width / 2, pos.y + box.height / 2, 0 };
    
    // Top right
    if (edgeTopLeft)
    {
        posData[i++] = { pos.x, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y, 0 };
        drawImage(edgeImage, { pos.x + edgeWidth, pos.y + edgeHeight }, color, 180.f);
    }
    else
        posData[i++] = { pos.x, pos.y, 0 };
    
    // Top left
    if (edgeTopRight)
    {
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + box.width, pos.y + edgeHeight, 0 };
        drawImage(edgeImage, { pos.x + box.width - edgeWidth, pos.y + edgeHeight }, color, -90.f);
    }
    else
        posData[i++] = { pos.x + box.width, pos.y, 0 };
    
    // Bottom left
    if (edgeBottomRight)
    {
        posData[i++] = { pos.x + box.width, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + box.height, 0 };
        drawImage(edgeImage, { pos.x + box.width - edgeWidth, pos.y + box.height - edgeHeight }, color, 0.f);
    }
    else
        posData[i++] = { pos.x + box.width, pos.y + box.height, 0 };
    
    // Bottom right
    if (edgeBottomLeft)
    {
        posData[i++] = { pos.x + edgeWidth, pos.y + box.height, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x, pos.y + box.height - edgeHeight, 0 };
        drawImage(edgeImage, { pos.x + edgeWidth, pos.y + box.height - edgeHeight }, color, 90.f);
    }
    else
        posData[i++] = { pos.x, pos.y + box.height, 0 };
    
    // Final
    if (edgeTopLeft)
        posData[i++] = { pos.x, pos.y + edgeHeight, 0 };
    else
        posData[i++] = { pos.x, pos.y, 0 };
    
    // No texture data (solid color texture)
    for (int i = 0; i < numVertex; i++)
        texCoordData[i] = { 0, 0 };
    
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, posData, texCoordData, numVertex,
                                        appHandler->solidColor->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_TRIANGLE_FAN);*/
}

EXPORT void Base::drawLine(ScreenPos start, ScreenPos end, Color color, int thickness)
{
    Vertex2Di vertexData[2] = {
        { { start.x, start.y }, { 0, 0 } },
        { { end.x, end.y },     { 0, 0 } }
    };
    
    glLineWidth(thickness);
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, vertexData , 2,
                                        appHandler->solidColor->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_LINES);
}