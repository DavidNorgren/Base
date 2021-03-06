#pragma once

#include "util/data/vec2.hpp"
#include "util/data/region2d.hpp"


namespace Base
{
    /* Returns the mouse position. */
    EXPORT ScreenPos mousePos();
    EXPORT ScreenPos mousePosPrevious();
    EXPORT ScreenPos mousePosClick();
    EXPORT Vec2i mouseMove();
    
    /* Returns whether the mouse is in a box. */
    EXPORT bool mouseInBox(const ScreenArea& box);
    
    /* Returns the state of the left mouse button. */
    EXPORT bool mouseLeftDown();
    EXPORT bool mouseLeftPressed();
    EXPORT bool mouseLeftReleased();
    EXPORT double mouseLastClickDuration();

    /* Returns the state of the right mouse button. */
    EXPORT bool mouseRightDown();
    EXPORT bool mouseRightPressed();
    EXPORT bool mouseRightReleased();

    /* Returns the mouse wheel state. */
    EXPORT Vec2f mouseScroll();

    /* Clears the mouse state. */
    EXPORT void mouseClear();
}