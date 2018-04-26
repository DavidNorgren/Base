#pragma once

#include "util/data/vec2.hpp"
#include "util/data/region2d.hpp"
#include "render/color.hpp"
#include "render/colors.hpp"
#include "resource/image.hpp"
#include "resource/font.hpp"


/* drawfunc - 2D drawing functionality */
namespace Base
{
    /* Text alignment. */
    enum class TextAlignX { LEFT, CENTER, RIGHT };
    enum class TextAlignY { TOP, MIDDLE, BOTTOM };
    struct TextAlign
    {
        TextAlignX x;
        TextAlignY y;
    };

    /* Begins drawing. */
    EXPORT void drawBegin();

    /* Clears using a drawing color. */
    EXPORT void drawClear(Color color);

    /* Sets a render area. */
    EXPORT void setDrawingArea(ScreenArea area);
    EXPORT ScreenArea getDrawingArea();
    EXPORT void resetDrawingArea();

    /* Set alpha (opacity) to use in subsequent drawing. */
    EXPORT void setDrawingAlpha(float alpha);
    EXPORT float getDrawingAlpha();

    /* Draws a piece of text using the drawing font. */
    EXPORT void drawText(string text, ScreenPos pos,             Color color = Colors::BLACK);
    EXPORT void drawText(string text, ScreenPos pos, Font* font, Color color = Colors::BLACK);
    EXPORT void drawTextAligned(string text, ScreenPos pos,             TextAlignX alignX = TextAlignX::LEFT, TextAlignY alignY = TextAlignY::TOP, Color color = Colors::BLACK);
    EXPORT void drawTextAligned(string text, ScreenPos pos, Font* font, TextAlignX alignX = TextAlignX::LEFT, TextAlignY alignY = TextAlignY::TOP, Color color = Colors::BLACK);

    /* Draws an image. */
    EXPORT void drawImage(string name,  ScreenPos pos, Color color = Colors::WHITE, float rotation = 0.f, Vec2f scale = { 1.f, 1.f });
    EXPORT void drawImage(Image* image, ScreenPos pos, Color color = Colors::WHITE, float rotation = 0.f, Vec2f scale = { 1.f, 1.f });
    EXPORT void drawSubImage(string image, int subImage, ScreenPos pos, Color color = Colors::WHITE, float rotation = 0.f, Vec2f scale = { 1.f, 1.f });
    EXPORT void drawSubImage(Image* image, int subImage, ScreenPos pos, Color color = Colors::WHITE, float rotation = 0.f, Vec2f scale = { 1.f, 1.f });

    /* Draws a colored box. */
    EXPORT void drawBox(ScreenArea box, Color color, bool outline = false, int outlineThickness = 1);
    EXPORT void drawBoxEdges(ScreenArea box, Color color, string edgeImage, bool edgeTopLeft = true, bool edgeTopRight = true, bool edgeBottomRight = true, bool edgeBottomLeft = true);
    
    /* Draws a line */
    EXPORT void drawLine(ScreenPos start, ScreenPos end, Color color, int thickness = 1);
}