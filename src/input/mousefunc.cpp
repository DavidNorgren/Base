#include "common.hpp"
#include "input/mousefunc.hpp"
#include "apphandler.hpp"


Base::ScreenPos Base::mousePos()
{
    return appHandler->mainWindow->mousePos;
}

Base::ScreenPos Base::mousePosPrevious()
{
    return appHandler->mainWindow->mousePosPrevious;
}

Base::ScreenPos Base::mousePosClick()
{
    return appHandler->mainWindow->mousePosClick;
}

Base::Vec2i Base::mouseMove()
{
    return appHandler->mainWindow->mouseMove;
}

bool Base::mouseInBox(const ScreenArea& box)
{
    ScreenPos mouse = appHandler->mainWindow->mousePos;
    return (mouse.x >= box.pos.x && mouse.x < box.pos.x + box.width &&
            mouse.y >= box.pos.y && mouse.y < box.pos.y + box.height);
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

Base::Vec2f Base::mouseScroll()
{
    return appHandler->mainWindow->mouseScroll;
}

void Base::mouseClear()
{
    Window* window = appHandler->mainWindow;
    for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
    {
        window->mouseDown[m] = false;
        window->mousePressed[m] = false;
        window->mouseReleased[m] = false;
    }
    mouseSetCursor(Cursor::ARROW);
}

void Base::mouseSetCursor(Cursor cursor)
{
    GLFWcursor* glfwcursor;
    switch (cursor)
    {
        case Cursor::ARROW:     glfwcursor = appHandler->mainWindow->cursorArrow;     break;
        case Cursor::HANDPOINT: glfwcursor = appHandler->mainWindow->cursorHandpoint; break;
        case Cursor::BEAM:      glfwcursor = appHandler->mainWindow->cursorBeam;      break;
        case Cursor::WERESIZE:  glfwcursor = appHandler->mainWindow->cursorHResize;   break;
        case Cursor::NSRESIZE:  glfwcursor = appHandler->mainWindow->cursorVResize;   break;
    }
    appHandler->mainWindow->currentCursor = glfwcursor;
}