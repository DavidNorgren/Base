#include "common.hpp"
#include "apphandler.hpp"
#include "render/drawfunc.hpp"
#include "render/renderfunc.hpp"


EXPORT void Base::drawBegin()
{
    appHandler->drawingShader->select();
    resetDrawingArea();
}

EXPORT void Base::drawClear(const Color& color)
{
    glClearColor(color.r, color.g, color.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

EXPORT void Base::setDrawingArea(const ScreenArea& area)
{
    glEnable(GL_SCISSOR_TEST);
    appHandler->drawingArea = area;
    glScissor(area.x, getRenderTarget()->getSize().height - (area.y + area.height), area.width, area.height);
}

EXPORT Base::ScreenArea Base::getDrawingArea()
{
    return appHandler->drawingArea;
}

EXPORT void Base::resetDrawingArea()
{
    appHandler->drawingArea.pos = { 0, 0 };
    appHandler->drawingArea.size = getRenderTarget()->getSize();
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

EXPORT void Base::drawText(const string& text, const ScreenPos& pos, const Color& color)
{
    drawText(text, pos, appHandler->drawingFont, color);
}

EXPORT void Base::drawText(const string& text, const ScreenPos& pos, Font* font, const Color& color)
{
    Mat4f mat = getRenderTarget()->getOrtho2D() *
                Mat4f::translate({ (float)pos.x, (float)pos.y, 0 });
    
    appHandler->drawingShader->render2D(mat, font->getTextVertices(text), font, Color(color, appHandler->drawingAlpha));
}

EXPORT void Base::drawTextAligned(const string& text, const ScreenPos& pos, const TextAlign& align, const Color& color)
{
    drawTextAligned(text, pos, appHandler->drawingFont, align, color);
}

EXPORT void Base::drawTextAligned(const string& text, const ScreenPos& pos, Font* font, const TextAlign& align, const Color& color)
{
    ScreenPos nPos = pos;

    // Vertical alignment
    if (align.y != TextAlign::TOP)
    {
        int height = font->getTextHeight(text);
        
        if (align.y == TextAlign::MIDDLE)
            nPos.y -= height / 2;

        else if (align.y == TextAlign::BOTTOM)
            nPos.y -= height;
    }
        
    // Horizontal alignment
    if (align.x == TextAlign::LEFT)
        drawText(text, nPos, font, color);
    else
    {
        for (string line : stringSplit(text, "\n"))
        {
            int width = font->getTextWidth(line);
            
            if (align.x == TextAlign::CENTER)
                nPos.x -= width / 2;

            else if (align.x == TextAlign::RIGHT)
                nPos.x -= width;
            
            drawText(line, nPos, font, color);
        }
    }
}

EXPORT void Base::drawImage(const string& name, const ScreenPos& pos, const Color& color, float rotation, const Vec2f& scale)
{
    drawImage((Image*)appHandler->resHandler->get(name), pos, color, rotation, scale);
}

EXPORT void Base::drawImage(Image* image, const ScreenPos& pos, const Color& color, float rotation, const Vec2f& scale)
{
    Size2Di size = image->getSize();
    List<Vertex2Di> vertexData({
        { { 0, 0 },                    { 0, 0 } },
        { { 0, size.height },          { 0, 1 } },
        { { size.width, 0 },           { 1, 0 } },
        { { size.width, size.height }, { 1, 1 } }
    });
    
    Mat4f mat = getRenderTarget()->getOrtho2D() *
                Mat4f::translate({ (float)pos.x, (float)pos.y, 0 }) *
                Mat4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                Mat4f::scale({ scale.x, scale.y, 1.f });

    appHandler->drawingShader->render2D(mat, vertexData, image, Color(color, appHandler->drawingAlpha), GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawSubImage(const string& name, int subImage, const ScreenPos& pos, const Color& color, float rotation, const Vec2f& scale)
{
    drawSubImage((Image*)appHandler->resHandler->get(name), subImage, pos, color, rotation, scale);
}

EXPORT void Base::drawSubImage(Image* image, int subImage, const ScreenPos& pos, const Color& color, float rotation, const Vec2f& scale)
{
    Size2Di size    = image->getSize();
    int subImages   = size.width / size.height;
    Tex2f texStart  = { (float)subImage / (float)subImages, 0.f };
    Tex2f texEnd    = { texStart.x + (float)size.height / size.width, 1.f };
    
    List<Vertex2Di> vertexData({
        { { 0, 0 },                     texStart },
        { { 0, size.height },           { texStart.x, texEnd.y } },
        { { size.height, 0 },           { texEnd.x, texStart.y } },
        { { size.height, size.height }, texEnd }
    });
    
    Mat4f mat = getRenderTarget()->getOrtho2D() *
                Mat4f::translate({ (float)pos.x, (float)pos.y, 0 }) *
                Mat4f::rotate({ 0.f, 0.f, 1.f }, rotation) *
                Mat4f::scale({ scale.x, scale.y, 1.f });
                 
    appHandler->drawingShader->render2D(mat, vertexData, image, Color(color, appHandler->drawingAlpha), GL_TRIANGLE_STRIP);
}

EXPORT void Base::drawBox(const ScreenArea& box, const Color& color, bool outline, int outlineThickness)
{
    List<Vertex2Di> vertexData({
        { { box.x + box.width, box.y },              { 0, 0 } },
        { { box.x, box.y },                          { 0, 0 } },
        { { box.x, box.y + box.height },             { 0, 0 } },
        { { box.x + box.width, box.y + box.height }, { 0, 0 } },
        { { box.x + box.width, box.y },              { 0, 0 } }
    });

    glLineWidth(outlineThickness);
    appHandler->drawingShader->render2D(getRenderTarget()->getOrtho2D(), vertexData,
                                        appHandler->solidColor, Color(color, appHandler->drawingAlpha),
                                        outline ? GL_LINE_STRIP : GL_TRIANGLE_FAN);
}

EXPORT void Base::drawLine(const ScreenPos& start, const ScreenPos& end, const Color& color, int thickness)
{
    List<Vertex2Di> vertexData({
        { { start.x, start.y }, { 0, 0 } },
        { { end.x, end.y },     { 0, 0 } }
    });
    
    glLineWidth(thickness);
    appHandler->drawingShader->render2D(getRenderTarget()->getOrtho2D(), vertexData,
                                        appHandler->solidColor, Color(color, appHandler->drawingAlpha),
                                        GL_LINES);
}