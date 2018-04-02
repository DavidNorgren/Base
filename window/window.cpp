#include "window/window.hpp"

#include <iostream>

Base::Window* w;

void windowMouseButtonCallback(GLFWwindow* handle, int button, int action, int mods)
{
    if (button < 0)
        return;

    // A mouse button was pressed!
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            w->mouseLastClickDuration = glfwGetTime() - w->mouselastClickTime;
            w->mouselastClickTime = glfwGetTime();
            w->mousePosClick = w->mousePos;
        }

        if (!w->mouseDown[button])
            w->mousePressed[button] = true;
        w->mouseDown[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (w->mouseDown[button])
            w->mouseReleased[button] = true;
        w->mouseDown[button] = false;
    }
    
    // Call event and set cursor
    if (w->mouseEventFunc)
    {
        w->currentCursor = w->cursorArrow;
        w->mouseEventFunc();
        w->setCursor(w->currentCursor);
    }
}

void windowCursorPosCallback(GLFWwindow* handle, double x, double y)
{
    // Mouse was moved!
    w->mousePosPrevious = w->mousePos;
    w->mousePos = { (int)x, (int)y };
    w->mouseMove = Base::Vec2(w->mousePos.x - w->mousePosPrevious.x,
                              w->mousePos.y - w->mousePosPrevious.y);
    
    // Call event and set cursor
    if (w->mouseEventFunc)
    {
        w->currentCursor = w->cursorArrow;
        w->mouseEventFunc();
        w->setCursor(w->currentCursor);
    }
}

void windowKeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    if (key < 0)
        return;

    // A keyboard key was pressed!
    if (action == GLFW_PRESS)
    {
        if (!w->keyDown[key])
            w->keyPressed[key] = true;
        w->keyDown[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (w->keyDown[key])
            w->keyReleased[key] = true;
        w->keyDown[key] = false;
    }
    else if (action == GLFW_REPEAT)
        w->keyPressed[key] = true;
    
    if (w->keyEventFunc)
        w->keyEventFunc();
}

void windowCharModsCallback(GLFWwindow* handle, unsigned int codepoint, int mods)
{
    w->charPressed = (char)codepoint;
    if (w->keyEventFunc)
        w->keyEventFunc();
    w->charPressed = 0;
}

void windowSizeCallback(GLFWwindow* handle, int width, int height)
{
    // The window was resized!
    width = Base::max(1, width);
    height = Base::max(1, height);

    glViewport(0, 0, width, height);
    w->ortho = Base::Mat4x4::ortho(0.f, width, height, 0.f, 0.f, 1.f);
    w->width = width;
    w->height = height;
    w->ratio = (float)width / height;
    
    if (w->resizeEventFunc)
        w->resizeEventFunc();
}

Base::Window::Window()
{
    // Initialize window
    handle = glfwCreateWindow(640, 480, "", NULL, NULL);
    glfwMakeContextCurrent(handle);
    w = this;

    // Set callbacks
    glfwSetMouseButtonCallback(handle, windowMouseButtonCallback);
    glfwSetCursorPosCallback(handle, windowCursorPosCallback);
    glfwSetKeyCallback(handle, windowKeyCallback);
    glfwSetCharModsCallback(handle, windowCharModsCallback);
    glfwSetWindowSizeCallback(handle, windowSizeCallback);
    
    // Initialize input
    charPressed = 0;
    for (uint k = 0; k < GLFW_KEY_LAST; k++)
    {
        keyDown[k] = false;
        keyPressed[k] = false;
        keyReleased[k] = false;
    }

    for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
    {
        mouseDown[m] = false;
        mousePressed[m] = false;
        mouseReleased[m] = false;
    }
    
    // Initialize cursors
    cursorArrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursorHandpoint = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    cursorBeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursorHResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    cursorVResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    currentCursor = cursorArrow;
}

void Base::Window::open(std::function<void()> loopEventFunc,
                        std::function<void()> mouseEventFunc,
                        std::function<void()> keyEventFunc,
                        std::function<void()> resizeEventFunc)
{
    this->loopEventFunc = loopEventFunc;
    this->mouseEventFunc = mouseEventFunc;
    this->keyEventFunc = keyEventFunc;
    this->resizeEventFunc = resizeEventFunc;
    
    // Init FPS counter
    int lastTime = -1, frame = 0;
    fps = 0;
    
    // Maximize
    maximize();

    while (!glfwWindowShouldClose(handle))
    {
        // Clear screen
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
        // Call loop function
        loopEventFunc();

        // Reset input
        for (uint k = 0; k < GLFW_KEY_LAST; k++)
        {
            keyPressed[k] = false;
            keyReleased[k] = false;
        }

        for (uint m = 0; m < GLFW_MOUSE_BUTTON_LAST; m++)
        {
            mousePressed[m] = false;
            mouseReleased[m] = false;
        }

        w->mouseMove = { 0, 0 };

        // Update FPS
        frame++;
        if (lastTime != (int)glfwGetTime())
        {
            fps = frame;
            frame = 0;
        }
        lastTime = (int)glfwGetTime();

        // Swap buffers
        glfwSwapBuffers(handle);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Base::Window::maximize()
{
//#ifdef _WIN32
    glfwMaximizeWindow(handle);
/*#else
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(handle, 25, 25);
    glfwSetWindowSize(handle, mode->width - 50, mode->height - 100);
#endif*/
}

void Base::Window::setTitle(string title)
{
    glfwSetWindowTitle(handle, &title[0]);
}

void Base::Window::setCursor(GLFWcursor* cursor)
{
    glfwSetCursor(handle, cursor);
}