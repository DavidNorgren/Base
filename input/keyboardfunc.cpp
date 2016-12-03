#include "input/keyboardfunc.hpp"
#include "apphandler.hpp"


bool Base::keyDown(int key)
{
    return appHandler->mainWindow->keyDown[key];
}


bool Base::keyPressed(int key)
{
    return appHandler->mainWindow->keyPressed[key];
}


bool Base::keyReleased(int key)
{
    return appHandler->mainWindow->keyReleased[key];
}


char Base::charPressed()
{
    return appHandler->mainWindow->charPressed;
}