#include "common.hpp"
#include "input/mousefunc.hpp"
#include "apphandler.hpp"


Base::ScreenPos Base::mousePos()
{
    return appHandler->mainWindow->getMousePos();
}

Base::ScreenPos Base::mousePosPrevious()
{
    return appHandler->mainWindow->getMousePosPrevious();
}

Base::ScreenPos Base::mousePosClick()
{
    return appHandler->mainWindow->getMousePosClick();
}

Base::Vec2i Base::mouseMove()
{
    return appHandler->mainWindow->getMouseMove();
}

bool Base::mouseInBox(const ScreenArea& box)
{
    ScreenPos mouse = appHandler->mainWindow->getMousePos();
    return (mouse.x >= box.pos.x && mouse.x < box.pos.x + box.width &&
            mouse.y >= box.pos.y && mouse.y < box.pos.y + box.height);
}

bool Base::mouseLeftDown()
{
    return appHandler->mainWindow->getMouseDown(GLFW_MOUSE_BUTTON_LEFT);
}

bool Base::mouseLeftPressed()
{
    return appHandler->mainWindow->getMousePressed(GLFW_MOUSE_BUTTON_LEFT);
}

bool Base::mouseLeftReleased()
{
    return appHandler->mainWindow->getMouseReleased(GLFW_MOUSE_BUTTON_LEFT);
}

double Base::mouseLastClickDuration()
{
    return appHandler->mainWindow->getMouseLastClickDuration();
}

bool Base::mouseRightDown()
{
    return appHandler->mainWindow->getMouseDown(GLFW_MOUSE_BUTTON_RIGHT);
}

bool Base::mouseRightPressed()
{
    return appHandler->mainWindow->getMousePressed(GLFW_MOUSE_BUTTON_RIGHT);
}

bool Base::mouseRightReleased()
{
    return appHandler->mainWindow->getMouseReleased(GLFW_MOUSE_BUTTON_RIGHT);
}

Base::Vec2f Base::mouseScroll()
{
    return appHandler->mainWindow->getMouseScroll();
}

void Base::mouseClear()
{
    appHandler->mainWindow->mouseClear();
}