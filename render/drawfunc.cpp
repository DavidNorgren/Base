#include "apphandler.hpp"
#include "render/drawfunc.hpp"


void Base::drawBegin()
{
    appHandler->drawingShader->select();
}


void Base::drawText(string text, ScreenPos pos, Color color)
{
    drawText(text, appHandler->drawingFont, pos, color);
}


void Base::drawText(string text, Font* font, ScreenPos pos, Color color)
{
    // Init position and texture coordinate buffers
    uint textLength = text.size();
    Vec3 posData[textLength * 6];
    Vec2 texCoordData[textLength * 6];
    ScreenPos charPos = { 0, 0 };

    for (uint c = 0; c < textLength; c++)
    {
        uchar curChar = text[c];

        if (curChar == '\n') {
            charPos.x = 0;
            charPos.y += font->height * LINE_SPACE;
            continue;
        }

        if (curChar < font->start || curChar > font->end) {
            continue;
        }

        CharInfo curCharInfo = font->chars[curChar];

        if (curCharInfo.width && curCharInfo.height) {
            float vx = charPos.x + curCharInfo.left;
            float vy = charPos.y + font->height - curCharInfo.top;
            float vw = curCharInfo.width;
            float vh = curCharInfo.height;
            float tx = curCharInfo.mapX / font->width;
            float tw = curCharInfo.width / font->width;
            float th = curCharInfo.height / font->height;

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

        charPos += { (int)curCharInfo.advanceX, (int)curCharInfo.advanceY };
    }

    Mat4x4 mat = appHandler->mainWindow->ortho *
                 Mat4x4::translate({ pos.x, pos.y, 0 });
                 
    appHandler->drawingShader->render2D(mat, posData, texCoordData, textLength * 6, font->texture, color);
}


void Base::drawTextAligned(string text, ScreenPos pos, TextAlignX alignX, TextAlignY alignY, Color color)
{
    drawTextAligned(text, appHandler->drawingFont, pos, alignX, alignY, color);
}


void Base::drawTextAligned(string text, Font* font, ScreenPos pos, TextAlignX alignX, TextAlignY alignY, Color color)
{
    // Vertical alignment
    if (alignY != TOP)
    {
        int height = font->stringGetHeight(text);
        
        if (alignY == MIDDLE) {
            pos.y -= height / 2;
        }
        else if (alignY == BOTTOM) {
            pos.y -= height;
        }
        
    }
        
    // Horizontal alignment
    if (alignX == LEFT) {
        drawText(text, font, pos, color);
    }
    else {
        string_list lines = stringSplit(text, "\n");
        for (string line : lines)
        {
            int width = font->stringGetWidth(line);
            
            if (alignX == CENTER) {
                pos.x -= width / 2;
            }
            else if (alignX == RIGHT) {
                pos.x -= width;
            }
            
            drawText(line, font, pos, color);
        }
    }
}


void Base::drawImage(string name, ScreenPos pos, Color color, float rotation, Vec2 scale)
{
    drawImage((Image*)appHandler->resourceHandler->find(name)->loaded, pos, color, rotation, scale);
}


void Base::drawImage(Image* image, ScreenPos pos, Color color, float rotation, Vec2 scale)
{
    Vec3 posData[4] = {
        { 0.f },
        { (float)image->width, 0.f, 0.f },
        { 0.f, (float)image->height, 0.f },
        { (float)image->width, (float)image->height, 0.f },
    };
    Vec2 texCoordData[4] = {
        { 0, 0 },
        { 1, 0 },
        { 0, 1 },
        { 1, 1 }
    };
    
    Mat4x4 mat = appHandler->mainWindow->ortho *
                 Mat4x4::translate({ pos.x, pos.y, 0 }) *
                 Mat4x4::rotate({ 0.f, 0.f, 1.f }, rotation) *
                 Mat4x4::scale({ scale.x, scale.y, 1.f });

    appHandler->drawingShader->render2D(mat, posData, texCoordData, 4, image->texture, color, GL_TRIANGLE_STRIP);
}


void Base::drawSubImage(string name, int subImage, ScreenPos pos, Color color, float rotation, Vec2 scale)
{
    drawSubImage((Image*)appHandler->resourceHandler->find(name)->loaded, subImage, pos, color, rotation, scale);
}


void Base::drawSubImage(Image* image, int subImage, ScreenPos pos, Color color, float rotation, Vec2 scale)
{
    int subImages = image->width / image->height;
    Vec2 texStart = { (float)subImage / (float)subImages, 0.f };
    Vec2 texEnd = { texStart.x + (float)image->height / (float)image->width, 1.f };
    
    Vec3 posData[4] = {
        { 0.f },
        { (float)image->height, 0.f, 0.f },
        { 0.f, (float)image->height, 0.f },
        { (float)image->height, (float)image->height, 0.f },
    };
    Vec2 texCoordData[4] = {
        texStart,
        { texEnd.x, texStart.y },
        { texStart.x, texEnd.y },
        texEnd
    };
    
    Mat4x4 mat = appHandler->mainWindow->ortho *
                 Mat4x4::translate({ pos.x, pos.y, 0 }) *
                 Mat4x4::rotate({ 0.f, 0.f, 1.f }, rotation) *
                 Mat4x4::scale({ scale.x, scale.y, 1.f });
                 
    appHandler->drawingShader->render2D(mat, posData, texCoordData, 4, image->texture, color, GL_TRIANGLE_STRIP);
}


void Base::drawBox(ScreenArea box, Color color, bool outline)
{
    ScreenPos& pos = box.pos;
    Vec3 posData[5] = {
        { pos.x, pos.y, 0 },
        { pos.x + box.width, pos.y, 0 },
        { pos.x + box.width, pos.y + box.height, 0 },
        { pos.x, pos.y + box.height, 0 },
        { pos.x, pos.y, 0 }
    };
    
    // No texture data (solid color texture))
    Vec2 texCoordData[5];
    for (int i = 0; i < 5; i++) {
        texCoordData[i] = { 0, 0 };
    }

    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, posData, texCoordData, 5, appHandler->solidTexture->texture, color, outline ? GL_LINE_STRIP : GL_TRIANGLE_STRIP);
}


void Base::drawBoxEdges(ScreenArea box, Color color, string edgeImage, bool edgeTopLeft, bool edgeTopRight, bool edgeBottomRight, bool edgeBottomLeft)
{
    Image* edge = (Image*)appHandler->resourceHandler->find(edgeImage)->loaded;
    int edgeWidth = edge->width;
    int edgeHeight = edge->height;
    int i = 0;
    int numVertex = 2 +
                    (1 + 2 * edgeTopRight) +
                    (1 + 2 * edgeTopLeft) +
                    (1 + 2 * edgeBottomLeft) +
                    (1 + 2 * edgeBottomRight);
    
    ScreenPos& pos = box.pos;
    Vec3 posData[numVertex];
    Vec2 texCoordData[numVertex];
    
    // Center vertex
    posData[i++] = { pos.x + box.width / 2, pos.y + box.height / 2, 0 };
    
    // Top right
    if (edgeTopLeft) {
        posData[i++] = { pos.x, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y, 0 };
        drawImage(edgeImage, { pos.x + edgeWidth, pos.y + edgeHeight }, color, 180.f);
    }
    else {
        posData[i++] = { pos.x, pos.y, 0 };
    }
    
    // Top left
    if (edgeTopRight) {
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + edgeHeight, 0 };
        posData[i++] = { pos.x + box.width, pos.y + edgeHeight, 0 };
        drawImage(edgeImage, { pos.x + box.width - edgeWidth, pos.y + edgeHeight }, color, -90.f);
    }
    else {
        posData[i++] = { pos.x + box.width, pos.y, 0 };
    }
    
    // Bottom left
    if (edgeBottomRight) {
        posData[i++] = { pos.x + box.width, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x + box.width - edgeWidth, pos.y + box.height, 0 };
        drawImage(edgeImage, { pos.x + box.width - edgeWidth, pos.y + box.height - edgeHeight }, color, 0.f);
    }
    else {
        posData[i++] = { pos.x + box.width, pos.y + box.height, 0 };
    }
    
    // Bottom right
    if (edgeBottomLeft) {
        posData[i++] = { pos.x + edgeWidth, pos.y + box.height, 0 };
        posData[i++] = { pos.x + edgeWidth, pos.y + box.height - edgeHeight, 0 };
        posData[i++] = { pos.x, pos.y + box.height - edgeHeight, 0 };
        drawImage(edgeImage, { pos.x + edgeWidth, pos.y + box.height - edgeHeight }, color, 90.f);
    }
    else {
        posData[i++] = { pos.x, pos.y + box.height, 0 };
    }
    
    // Final
    if (edgeTopLeft) {
        posData[i++] = { pos.x, pos.y + edgeHeight, 0 };
    }
    else {
        posData[i++] = { pos.x, pos.y, 0 };
    }
    
    // No texture data (solid color texture))
    for (int i = 0; i < numVertex; i++) {
        texCoordData[i] = { 0, 0 };
    }
    
    appHandler->drawingShader->render2D(appHandler->mainWindow->ortho, posData, texCoordData, numVertex, appHandler->solidTexture->texture, color, GL_TRIANGLE_FAN);
}