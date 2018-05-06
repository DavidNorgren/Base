#include "common.hpp"
#include "util/mathfunc.hpp"
#include "render/drawfunc.hpp"
#include "window/window.hpp"


// GLFW callbacks
Map<GLFWwindow*, Base::Window*> glfwMap;

void windowMouseButtonCallback(GLFWwindow* handle, int button, int action, int mods)
{
    glfwMap[handle]->glfwMouseButtonCallback(button, action, mods);
}

void windowScrollCallback(GLFWwindow* handle, double x, double y)
{
    glfwMap[handle]->glfwScrollCallback(x, y);
}

void windowCursorPosCallback(GLFWwindow* handle, double x, double y)
{
    glfwMap[handle]->glfwCursorPosCallback(x, y);
}

void windowKeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    glfwMap[handle]->glfwKeyCallback(key, scancode, action, mods);
}

void windowCharModsCallback(GLFWwindow* handle, uint codepoint, int mods)
{
    glfwMap[handle]->glfwCharModsCallback(codepoint, mods);
}

void windowSizeCallback(GLFWwindow* handle, int width, int height)
{
    glfwMap[handle]->glfwSizeCallback(width, height);
}

EXPORT Base::Window::Window()
{
    // Initialize window
    size = { 1080, 800 };
    handle = glfwCreateWindow(size.width, size.height, "", NULL, NULL);
    glfwMakeContextCurrent(handle);
    glfwMap[handle] = this;
    
    // Set callbacks
    glfwSetMouseButtonCallback(handle, windowMouseButtonCallback);
    glfwSetScrollCallback(handle, windowScrollCallback);
    glfwSetCursorPosCallback(handle, windowCursorPosCallback);
    glfwSetKeyCallback(handle, windowKeyCallback);
    glfwSetCharModsCallback(handle, windowCharModsCallback);
    glfwSetWindowSizeCallback(handle, windowSizeCallback);
    
    // Initialize input
    charPressed = 0;
    for (uint k = 0; k < GLFW_KEY_LAST; k++)
    {
        keyDown[k]     = false;
        keyPressed[k]  = false;
        keyReleased[k] = false;
    }

    for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
    {
        mouseDown[m]     = false;
        mousePressed[m]  = false;
        mouseReleased[m] = false;
    }
    
    // Initialize cursors
    cursorArrow     = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursorHandpoint = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    cursorBeam      = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursorHResize   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    cursorVResize   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    currentCursor   = cursorArrow;
}

EXPORT void Base::Window::open(function<void()> loopEventFunc,
                               function<void()> mouseEventFunc,
                               function<void()> keyEventFunc,
                               function<void()> resizeEventFunc)
{
    this->loopEventFunc   = loopEventFunc;
    this->mouseEventFunc  = mouseEventFunc;
    this->keyEventFunc    = keyEventFunc;
    this->resizeEventFunc = resizeEventFunc;
    
    ///maximize();
    if (resizeEventFunc)
        resizeEventFunc();

    // Init FPS counter
    double lastTime = glfwGetTime();
    int frame = 0;
    
    // Limit framerate to monitor refresh rate
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(handle))
    {
        // Call loop function
        loopEventFunc();

        // Reset/update keyboard input
        bool isAnyDown = false;
        for (uint k = 0; k < GLFW_KEY_LAST; k++)
        {
            if (keyDown[k])
                isAnyDown = true;
            keyPressed[k]  = false;
            keyReleased[k] = false;
        }
        
        if (isAnyDown && keyEventFunc)
            keyEventFunc();

        // Reset/update mouse input
        isAnyDown = false;
        for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
        {
            if (mouseDown[m])
                isAnyDown = true;
            mousePressed[m]  = false;
            mouseReleased[m] = false;
        }

        mouseMove   = { 0, 0 };
        mouseScroll = { 0.f, 0.f };

        if (isAnyDown && mouseEventFunc)
            mouseEventFunc();

        // Swap buffers
        glfwSwapBuffers(handle);
        glfwPollEvents();

        // Calculate FPS from the number of frames rendered in the last second
        double curTime = glfwGetTime();
        frame++;
        fps = std::round(frame / (curTime - lastTime));
        if ((int)lastTime != (int)curTime)
        {
            fps      = frame;
            frame    = 0;
            lastTime = glfwGetTime();
        }
    }

    glfwTerminate();
}

EXPORT void Base::Window::maximize()
{
    glfwMaximizeWindow(handle);
}

EXPORT void Base::Window::setTitle(const string& title)
{
    glfwSetWindowTitle(handle, &title[0]);
}

EXPORT void Base::Window::setCursor(CursorStyle cursor)
{
    switch (cursor)
    {
        case CursorStyle::ARROW:     currentCursor = cursorArrow;     break;
        case CursorStyle::HANDPOINT: currentCursor = cursorHandpoint; break;
        case CursorStyle::BEAM:      currentCursor = cursorBeam;      break;
        case CursorStyle::WERESIZE:  currentCursor = cursorHResize;   break;
        case CursorStyle::NSRESIZE:  currentCursor = cursorVResize;   break;
    }
}

EXPORT float Base::Window::getFrameDelay() const
{
    if (fps > 0)
        return (float)targetFps / fps;

    return 1.f;
}

EXPORT void Base::Window::mouseClear()
{
    for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
    {
        mouseDown[m] = false;
        mousePressed[m] = false;
        mouseReleased[m] = false;
    }
}

void Base::Window::glfwMouseButtonCallback(int button, int action, int mods)
{
    if (button < 0)
        return;

    // A mouse button was pressed!
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            mouseLastClickDuration = glfwGetTime() - mouselastClickTime;
            mouselastClickTime = glfwGetTime();
            mousePosClick = mousePos;
        }

        if (!mouseDown[button])
            mousePressed[button] = true;
        mouseDown[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (mouseDown[button])
            mouseReleased[button] = true;
        mouseDown[button] = false;
    }
    
    // Call event and set cursor
    if (mouseEventFunc)
    {
        currentCursor = cursorArrow;
        mouseEventFunc();
        glfwSetCursor(handle, currentCursor);
    }
}

void Base::Window::glfwScrollCallback(double x, double y)
{
    mouseScroll = { (float)x, (float)y };
    if (mouseEventFunc)
        mouseEventFunc();
}

void Base::Window::glfwCursorPosCallback(double x, double y)
{
    // Mouse was moved!
    mousePosPrevious = mousePos;
    mousePos = { (int)x, (int)y };
    mouseMove = (mousePos - mousePosPrevious);
    
    // Call event and set cursor
    if (mouseEventFunc)
    {
        currentCursor = cursorArrow;
        mouseEventFunc();
        glfwSetCursor(handle, currentCursor);
    }
}

void Base::Window::glfwKeyCallback(int key, int scancode, int action, int mods)
{
    if (key < 0)
        return;

    // A keyboard key was pressed!
    if (action == GLFW_PRESS)
    {
        if (!keyDown[key])
            keyPressed[key] = true;
        keyDown[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (keyDown[key])
            keyReleased[key] = true;
        keyDown[key] = false;
    }
    else if (action == GLFW_REPEAT)
        keyPressed[key] = true;
    
    if (keyEventFunc)
        keyEventFunc();
}

void Base::Window::glfwCharModsCallback(uint codepoint, int mods)
{
    charPressed = (char)codepoint;
    if (keyEventFunc)
        keyEventFunc();
    charPressed = 0;
}

void Base::Window::glfwSizeCallback(int width, int height)
{
    // The window was resized!
    width  = max(1, width);
    height = max(1, height);
    size    = { width, height };
    
    if (resizeEventFunc)
        resizeEventFunc();
}