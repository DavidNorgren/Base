#pragma once

#include "window/screenarea.hpp"
#include "util/math/vec2.hpp"


namespace Base
{
    enum Cursor
    {
        ARROW,
        HANDPOINT,
        BEAM,
        WERESIZE,
        NSRESIZE
    };

    /* Returns the mouse position. */
    EXPORT ScreenPos mousePos();
    EXPORT ScreenPos mousePosPrevious();
    EXPORT ScreenPos mousePosClick();
    EXPORT Vec2 mouseMove();
    
    /* Returns whether the mouse is in a box. */
    EXPORT bool mouseInBox(ScreenArea box);
    
    /* Returns the state of the left mouse button. */
    EXPORT bool mouseLeftDown();
    EXPORT bool mouseLeftPressed();
    EXPORT bool mouseLeftReleased();
    EXPORT double mouseLastClickDuration();

    /* Returns the state of the right mouse button. */
    EXPORT bool mouseRightDown();
    EXPORT bool mouseRightPressed();
    EXPORT bool mouseRightReleased();

    /* Clears the mouse state. */
    EXPORT void mouseClear();
    
    /* Sets the mouse cursor. */
    EXPORT void mouseSetCursor(Cursor cursor);
}