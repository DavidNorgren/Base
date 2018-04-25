#pragma once

#include "GLFW/glfw3.h"

#include "util/data/mat4.hpp"
#include "util/data/region2d.hpp"
#include "render/colors.hpp"

namespace Base
{
    /* Opens and manages a resizable window. */
    class Window
    {
      public:
        EXPORT Window();

        EXPORT void open(function<void()> loopEventFunc,
                         function<void()> mouseEventFunc = nullptr,
                         function<void()> keyEventFunc = nullptr,
                         function<void()> resizeEventFunc = nullptr);

        EXPORT void maximize();

        EXPORT void setTargetFramerate(int fps);
        EXPORT void setTitle(string title);
        EXPORT void setCursor(GLFWcursor* cursor);
        EXPORT void setBackgroundColor(Color color);
        EXPORT float getFrameDelay();
        EXPORT Size2Di getSize();
        EXPORT float getRatio();
        
        /* Window size. */
        Size2Di size;
        float ratio;

        /* Background. */
        Color backgroundColor = Colors::BLACK;

        /* Window render framerate. */
        int fps, targetFps = 60;

        /* Mouse position and button status. */
        ScreenPos mousePos, mousePosPrevious, mousePosClick;
        Vec2i mouseMove;
        bool mouseDown[GLFW_MOUSE_BUTTON_LAST], mousePressed[GLFW_MOUSE_BUTTON_LAST], mouseReleased[GLFW_MOUSE_BUTTON_LAST];
        double mouselastClickTime = 0, mouseLastClickDuration = 0;

        /* Keyboard status. */
        bool keyDown[GLFW_KEY_LAST], keyPressed[GLFW_KEY_LAST], keyReleased[GLFW_KEY_LAST];
        char charPressed;
        
        /* Cursors. */
        GLFWcursor* cursorArrow;
        GLFWcursor* cursorHandpoint;
        GLFWcursor* cursorBeam;
        GLFWcursor* cursorHResize;
        GLFWcursor* cursorVResize;
        GLFWcursor* currentCursor;

        GLFWwindow* handle;
        function<void()> loopEventFunc, resizeEventFunc;
        function<void()> mouseEventFunc;
        function<void()> keyEventFunc;
        Mat4f ortho;
    };
}