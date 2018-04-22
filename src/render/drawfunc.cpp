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
    // Init position and texture coordinate buffers
    Vec3f posData[text.length() * 6];
    Vec2f texCoordData[text.length() * 6];

    ScreenPos charPos = { 0, 0 };
    for (uint c = 0; c < text.length(); c++)
    {
        uchar curChar = text[c];

        if (curChar == '\n' || curChar == '\r')
        {
            charPos.x = 0;
            charPos.y += font->glTextureSize.height * LINE_SPACE;
            continue;
        }

        if (curChar < font->start || curChar > font->end)
            continue;

        CharInfo curCharInfo = font->chars[curChar];

        if (curCharInfo.size.width && curCharInfo.size.height)
        {
            float vx = charPos.x + curCharInfo.pos.x;
            float vy = charPos.y + font->glTextureSize.height - curCharInfo.pos.y;
            float vw = curCharInfo.size.width;
            float vh = curCharInfo.size.height;
            float tx = curCharInfo.mapX / font->glTextureSize.width;
            float tw = (float)curCharInfo.size.width / font->glTextureSize.width;
            float th = (float)curCharInfo.size.height / font->glTextureSize.height;

            int i = c * 6;

            posData[i + 0] = { vx, vy, 0.f };
            posData[i + 1] = { vx, vy + vh, 0.f };
            posData[i + 2] = { vx + vw, vy, 0.f };
            posData[i + 3] = { vx + vw, vy, 0.f };
            posData[i + 4] = { vx, vy + vh, 0.f };
            posData[i + 5] = { vx + vw, vy + vh, 0.f };

            texCoordData[i + 0] = { tx, 0.f };
            texCoordData[i + 1] = { tx, th };
            texCoordData[i + 2] = { tx + tw, 0.f };
            texCoordData[i + 3] = { tx + tw, 0.f };
            texCoordData[i + 4] = { tx, th };
            texCoordData[i + 5] = { tx + tw, th };
        }

        charPos += curCharInfo.advance;
    }

    Mat4x4f mat = appHandler->mainWindow->ortho * Mat4x4f::translate({ pos.x, pos.y, 0 });
                 
    appHandler->drawingShader->render2D(mat, posData, texCoordData, text.length() * 6,
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
    drawImage((Image*)appHandler->resHandler->find(name)->loaded, pos, color, rotation, scale);
}

EXPORT void Base::drawImage(Image* image, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    Vec3f posData[4] = {
        { 0.f },
        { (float)image->glTextureSize.width, 0.f, 0.f },
        { 0.f, (float)image->glTextureSize.height, 0.f },
        { (float)image->glTextureSize.width, (float)image->glTextureSize.height, 0.f },
    };
    Vec2f texCoordData[4] = {
        { 0, 0 },
        { 1, 0 },
        { 0, 1 },
        { 1, 1 }
    };
    
    Mat4x4f mat = appHandler->mainWindow->ortho *
                  Mat4x4f::translate({ pos.x, pos.y, 0 }) *
                  Mat4x4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                  Mat4x4f::scale({ scale.x, scale.y, 1.f });

    appHandler->drawingShader->render2D(mat, posData, texCoordData, 4,
                                        image->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawSubImage(string name, int subImage, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    drawSubImage((Image*)appHandler->resHandler->find(name)->loaded, subImage, pos, color, rotation, scale);
}

EXPORT void Base::drawSubImage(Image* image, int subImage, ScreenPos pos, Color color, float rotation, Vec2f scale)
{
    int subImages = image->glTextureSize.width / image->glTextureSize.height;
    Vec2f texStart = { (float)subImage / (float)subImages, 0.f };
    Vec2f texEnd = { texStart.x + (float)image->glTextureSize.height / (float)image->glTextureSize.width, 1.f };
    
    Vec3f posData[4] = {
        { 0.f },
        { (float)image->glTextureSize.height, 0.f, 0.f },
        { 0.f, (float)image->glTextureSize.height, 0.f },
        { (float)image->glTextureSize.height, (float)image->glTextureSize.height, 0.f },
    };
    Vec2f texCoordData[4] = {
        texStart,
        { texEnd.x, texStart.y },
        { texStart.x, texEnd.y },
        texEnd
    };
    
    Mat4x4f mat = appHandler->mainWindow->ortho *
                Mat4x4f::translate({ pos.x, pos.y, 0 }) *
                Mat4x4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                Mat4x4f::scale({ scale.x, scale.y, 1.f });
                 
    appHandler->drawingShader->render2D(mat, posData, texCoordData, 4, 
                                        image->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawBox(ScreenArea box, Color color, bool outline, int outlineThickness)
{
    ScreenPos& pos = box.pos;
    Vec3f posData[5] = {
        { pos.x, pos.y, 0 },
        { pos.x + box.width, pos.y, 0 },
        { pos.x + box.width, pos.y + box.height, 0 },
        { pos.x, pos.y + box.height, 0 },
        { pos.x, pos.y, 0 }
    };
    
    // No texture data (solid color texture)
    Vec2f texCoordData[5];
    for (int i = 0; i < 5; i++)
        texCoordData[i] = { 0, 0 };

    glLineWidth(outlineThickness);
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, posData, texCoordData, 5,
                                        appHandler->solidColor->glTexture, Color(color, appHandler->drawingAlpha),
                                        outline ? GL_LINE_STRIP : GL_TRIANGLE_FAN);
}

EXPORT void Base::drawBoxEdges(ScreenArea box, Color color, string edgeImage, bool edgeTopLeft, bool edgeTopRight, bool edgeBottomRight, bool edgeBottomLeft)
{
    Image* edge = (Image*)appHandler->resHandler->find(edgeImage)->loaded;
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
                                        GL_TRIANGLE_FAN);
}

EXPORT void Base::drawLine(ScreenPos start, ScreenPos end, Color color, int thickness)
{
    Vec3f posData[2] = {
        { start.x, start.y, 0 },
        { end.x, end.y, 0 }
    };
    
    // No texture data (solid color texture)
    Vec2f texCoordData[2];
    texCoordData[0] = { 0, 0 };
    texCoordData[1] = { 0, 0 };
    
    glLineWidth(thickness);
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, posData, texCoordData, 2,
                                        appHandler->solidColor->glTexture, Color(color, appHandler->drawingAlpha),
                                        GL_LINES);
}