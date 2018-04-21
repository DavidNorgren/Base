#include "common.hpp"
#include "util/mathfunc.hpp"
#include "render/drawfunc.hpp"
#include "window/window.hpp"


Base::Window* window;

void windowMouseButtonCallback(GLFWwindow* handle, int button, int action, int mods)
{
    if (button < 0)
        return;

    // A mouse button was pressed!
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            window->mouseLastClickDuration = glfwGetTime() - window->mouselastClickTime;
            window->mouselastClickTime = glfwGetTime();
            window->mousePosClick = window->mousePos;
        }

        if (!window->mouseDown[button])
            window->mousePressed[button] = true;
        window->mouseDown[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (window->mouseDown[button])
            window->mouseReleased[button] = true;
        window->mouseDown[button] = false;
    }
    
    // Call event and set cursor
    if (window->mouseEventFunc)
    {
        window->currentCursor = window->cursorArrow;
        window->mouseEventFunc();
        window->setCursor(window->currentCursor);
    }
}

void windowCursorPosCallback(GLFWwindow* handle, double x, double y)
{
    // Mouse was moved!
    window->mousePosPrevious = window->mousePos;
    window->mousePos = { (int)x, (int)y };
    window->mouseMove = Base::Vec2i(window->mousePos.x - window->mousePosPrevious.x,
                               window->mousePos.y - window->mousePosPrevious.y);
    
    // Call event and set cursor
    if (window->mouseEventFunc)
    {
        window->currentCursor = window->cursorArrow;
        window->mouseEventFunc();
        window->setCursor(window->currentCursor);
    }
}

void windowKeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    if (key < 0)
        return;

    // A keyboard key was pressed!
    if (action == GLFW_PRESS)
    {
        if (!window->keyDown[key])
            window->keyPressed[key] = true;
        window->keyDown[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        if (window->keyDown[key])
            window->keyReleased[key] = true;
        window->keyDown[key] = false;
    }
    else if (action == GLFW_REPEAT)
        window->keyPressed[key] = true;
    
    if (window->keyEventFunc)
        window->keyEventFunc();
}

void windowCharModsCallback(GLFWwindow* handle, uint codepoint, int mods)
{
    window->charPressed = (char)codepoint;
    if (window->keyEventFunc)
        window->keyEventFunc();
    window->charPressed = 0;
}

void windowSizeCallback(GLFWwindow* handle, int width, int height)
{
    // The window was resized!
    width = Base::max(1, width);
    height = Base::max(1, height);

    glViewport(0, 0, width, height);
    window->ortho = Base::Mat4x4f::ortho(0.f, width, height, 0.f, 0.f, 1.f);
    window->size.width = width;
    window->size.height = height;
    window->ratio = (float)width / height;
    
    if (window->resizeEventFunc)
        window->resizeEventFunc();
}

EXPORT Base::Window::Window()
{
    // Initialize window
    handle = glfwCreateWindow(640, 480, "", NULL, NULL);
    glfwMakeContextCurrent(handle);
    window = this;

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

EXPORT void Base::Window::open(function<void()> loopEventFunc,
                               function<void()> mouseEventFunc,
                               function<void()> keyEventFunc,
                               function<void()> resizeEventFunc)
{
    this->loopEventFunc = loopEventFunc;
    this->mouseEventFunc = mouseEventFunc;
    this->keyEventFunc = keyEventFunc;
    this->resizeEventFunc = resizeEventFunc;
    
    // Init FPS counter
    int lastTime = -1, frame = 0;
    fps = 0;
    
    maximize();

    while (!glfwWindowShouldClose(handle))
    {
        // Clear screen
        drawClear(backgroundColor);
                
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

        window->mouseMove = { 0, 0 };

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

EXPORT void Base::Window::maximize()
{
//#ifdef _WIN32
    glfwMaximizeWindow(handle);
/*#else
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(handle, 25, 25);
    glfwSetWindowSize(handle, mode->width - 50, mode->height - 100);
#endif*/
}

EXPORT void Base::Window::setTitle(string title)
{
    glfwSetWindowTitle(handle, &title[0]);
}

EXPORT void Base::Window::setCursor(GLFWcursor* cursor)
{
    glfwSetCursor(handle, cursor);
}