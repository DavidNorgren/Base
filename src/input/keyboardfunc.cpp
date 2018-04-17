#include "input/keyboardfunc.hpp"
#include "apphandler.hpp"


EXPORT bool Base::keyDown(int key)
{
    return appHandler->mainWindow->keyDown[key];
}

EXPORT bool Base::keyPressed(int key)
{
    return appHandler->mainWindow->keyPressed[key];
}

EXPORT bool Base::keyReleased(int key)
{
    return appHandler->mainWindow->keyReleased[key];
}

EXPORT char Base::charPressed()
{
    return appHandler->mainWindow->charPressed;
}