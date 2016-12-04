#include "input/mousefunc.hpp"
#include "apphandler.hpp"


Base::ScreenPos Base::mousePos()
{
    return appHandler->mainWindow->mousePos;
}


Base::ScreenPos Base::mousePreviousPos()
{
    return appHandler->mainWindow->mousePosPrevious;
}


Base::Vec2 Base::mouseMove()
{
    return appHandler->mainWindow->mouseMove;
}


bool Base::mouseInBox(ScreenArea box)
{
    ScreenPos mouse = appHandler->mainWindow->mousePos;
    return (mouse.x >= box.pos.x && mouse.x < box.pos.x + box.width
         && mouse.y >= box.pos.y && mouse.y < box.pos.y + box.height);
}


bool Base::mouseLeftDown()
{
    return appHandler->mainWindow->mouseDown[GLFW_MOUSE_BUTTON_LEFT];
}


bool Base::mouseLeftPressed()
{
    return appHandler->mainWindow->mousePressed[GLFW_MOUSE_BUTTON_LEFT];
}


bool Base::mouseLeftReleased()
{
    return appHandler->mainWindow->mouseReleased[GLFW_MOUSE_BUTTON_LEFT];
}


double Base::mouseLastClickDuration()
{
    return appHandler->mainWindow->mouseLastClickDuration;
}


bool Base::mouseRightDown()
{
    return appHandler->mainWindow->mouseDown[GLFW_MOUSE_BUTTON_RIGHT];
}


bool Base::mouseRightPressed()
{
    return appHandler->mainWindow->mousePressed[GLFW_MOUSE_BUTTON_RIGHT];
}


bool Base::mouseRightReleased()
{
    return appHandler->mainWindow->mouseReleased[GLFW_MOUSE_BUTTON_RIGHT];
}


void Base::mouseClear()
{
    Window* window = appHandler->mainWindow;
    for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++) {
        window->mouseDown[m] = false;
        window->mousePressed[m] = false;
        window->mouseReleased[m] = false;
    }
    mouseSetCursor(ARROW);
}


void Base::mouseSetCursor(Cursor cursor)
{
    if (cursor == ARROW) {
        appHandler->mainWindow->currentCursor = appHandler->mainWindow->cursorArrow;
    }
    else if (cursor == HANDPOINT) {
        appHandler->mainWindow->currentCursor = appHandler->mainWindow->cursorHandpoint;
    }
    else if (cursor == BEAM) {
        appHandler->mainWindow->currentCursor = appHandler->mainWindow->cursorBeam;
    }
}