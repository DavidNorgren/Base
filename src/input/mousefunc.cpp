#include "common.hpp"
#include "input/mousefunc.hpp"
#include "apphandler.hpp"


EXPORT Base::ScreenPos Base::mousePos()
{
    return appHandler->mainWindow->getMousePos();
}

EXPORT Base::ScreenPos Base::mousePosPrevious()
{
    return appHandler->mainWindow->getMousePosPrevious();
}

EXPORT Base::ScreenPos Base::mousePosClick()
{
    return appHandler->mainWindow->getMousePosClick();
}

EXPORT Base::Vec2i Base::mouseMove()
{
    return appHandler->mainWindow->getMouseMove();
}

EXPORT bool Base::mouseInBox(const ScreenArea& box)
{
    ScreenPos mouse = appHandler->mainWindow->getMousePos();
    return (mouse.x >= box.pos.x && mouse.x < box.pos.x + box.width &&
            mouse.y >= box.pos.y && mouse.y < box.pos.y + box.height);
}

EXPORT bool Base::mouseLeftDown()
{
    return appHandler->mainWindow->getMouseDown(GLFW_MOUSE_BUTTON_LEFT);
}

EXPORT bool Base::mouseLeftPressed()
{
    return appHandler->mainWindow->getMousePressed(GLFW_MOUSE_BUTTON_LEFT);
}

EXPORT bool Base::mouseLeftReleased()
{
    return appHandler->mainWindow->getMouseReleased(GLFW_MOUSE_BUTTON_LEFT);
}

EXPORT double Base::mouseLastClickDuration()
{
    return appHandler->mainWindow->getMouseLastClickDuration();
}

EXPORT bool Base::mouseRightDown()
{
    return appHandler->mainWindow->getMouseDown(GLFW_MOUSE_BUTTON_RIGHT);
}

EXPORT bool Base::mouseRightPressed()
{
    return appHandler->mainWindow->getMousePressed(GLFW_MOUSE_BUTTON_RIGHT);
}

EXPORT bool Base::mouseRightReleased()
{
    return appHandler->mainWindow->getMouseReleased(GLFW_MOUSE_BUTTON_RIGHT);
}

EXPORT Base::Vec2f Base::mouseScroll()
{
    return appHandler->mainWindow->getMouseScroll();
}

EXPORT void Base::mouseClear()
{
    appHandler->mainWindow->mouseClear();
}