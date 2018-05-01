#pragma once

#include "util/data/vec2.hpp"
#include "util/data/region2d.hpp"
#include "render/color.hpp"
#include "render/colors.hpp"
#include "render/image.hpp"
#include "resource/font.hpp"


/* drawfunc - 2D drawing functionality */
namespace Base
{
    /* Text alignment. */
    struct TextAlign
    {
        enum { LEFT, CENTER, RIGHT } x;
        enum { TOP, MIDDLE, BOTTOM } y;
    };

    /* Begins drawing. */
    EXPORT void drawBegin();

    /* Clears using a drawing color. */
    EXPORT void drawClear(const Color& color);

    /* Sets a render area. */
    EXPORT void setDrawingArea(const ScreenArea& area);
    EXPORT ScreenArea getDrawingArea();
    EXPORT void resetDrawingArea();

    /* Set alpha (opacity) to use in subsequent drawing. */
    EXPORT void  setDrawingAlpha(float alpha);
    EXPORT float getDrawingAlpha();

    /* Draws a piece of text using the drawing font. */
    EXPORT void drawText(const string& text, const ScreenPos& pos,             const Color& color = Colors::BLACK);
    EXPORT void drawText(const string& text, const ScreenPos& pos, Font* font, const Color& color = Colors::BLACK);
    EXPORT void drawTextAligned(const string& text, const ScreenPos& pos,             const TextAlign& align, const Color& color = Colors::BLACK);
    EXPORT void drawTextAligned(const string& text, const ScreenPos& pos, Font* font, const TextAlign& align, const Color& color = Colors::BLACK);

    /* Draws an image. */
    EXPORT void drawImage(const string& name, const ScreenPos& pos, const Color& color = Colors::WHITE, float rotation = 0.f, const Vec2f& scale = { 1.f, 1.f });
    EXPORT void drawImage(Image* image,       const ScreenPos& pos, const Color& color = Colors::WHITE, float rotation = 0.f, const Vec2f& scale = { 1.f, 1.f });
    EXPORT void drawSubImage(const string& image, int subImage, const ScreenPos& pos, const Color& color = Colors::WHITE, float rotation = 0.f, const Vec2f& scale = { 1.f, 1.f });
    EXPORT void drawSubImage(Image* image,        int subImage, const ScreenPos& pos, const Color& color = Colors::WHITE, float rotation = 0.f, const Vec2f& scale = { 1.f, 1.f });

    /* Draws a colored box. */
    EXPORT void drawBox(const ScreenArea& box, const Color& color, bool outline = false, int outlineThickness = 1);
    
    /* Draws a line */
    EXPORT void drawLine(const ScreenPos& start, const ScreenPos& end, const Color& color, int thickness = 1);
}