#include "common.hpp"
#include "input/keyboardfunc.hpp"
#include "apphandler.hpp"


EXPORT bool Base::keyDown(int key)
{
    return appHandler->mainWindow->getKeyDown(key);
}

EXPORT bool Base::keyPressed(int key)
{
    return appHandler->mainWindow->getKeyPressed(key);
}

EXPORT bool Base::keyReleased(int key)
{
    return appHandler->mainWindow->getKeyReleased(key);
}

EXPORT char Base::charPressed()
{
    return appHandler->mainWindow->getCharPressed();
}