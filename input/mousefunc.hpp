#pragma once

#include "window/screenarea.hpp"


namespace Base
{
    
    enum Cursor {
        ARROW,
        HANDPOINT,
        BEAM
    };

    /* Returns whether the mouse is in a box. */
    bool mouseInBox(ScreenArea box);

    /* Returns the state of the left mouse button. */
    bool mouseLeftDown();
    bool mouseLeftPressed();
    bool mouseLeftReleased();

    /* Returns the state of the right mouse button. */
    bool mouseRightDown();
    bool mouseRightPressed();
    bool mouseRightReleased();

    /* Clears the mouse state. */
    void mouseClear();
    
    /* Sets the mouse cursor. */
    void mouseSetCursor(Cursor cursor);

}