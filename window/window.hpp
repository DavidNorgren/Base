#pragma once

#include <functional>

#include "GLFW/glfw3.h"

#include "util/math/mat4x4.hpp"
#include "util/math/mathfunc.hpp"
#include "util/stringfunc.hpp"
#include "window/screenpos.hpp"


namespace Base
{
    
    /* Opens and manages a resizable window. */
    class Window
    {
        public:

            /* Creates the window and OpenGL context. */
            Window();

            /* Opens the window. */
            void open(std::function<void()> loopEventFunc,
                      std::function<void()> mouseEventFunc = nullptr,
                      std::function<void()> keyEventFunc = nullptr,
                      std::function<void()> resizeEventFunc = nullptr);

            /* Sets the title of the window caption. */
            void setTitle(string title);
            
            /* Sets the mouse cursor. */
            void setCursor(GLFWcursor* cursor);

            /* Maximizes the window. */
            void maximize();

            /* Window size. */
            int width, height;
            float ratio;

            /* Window render FPS. */
            int fps;

            /* Mouse position and button status. */
            ScreenPos mouse, mousePrevious, mouseMove;
            bool mouseDown[GLFW_MOUSE_BUTTON_LAST], mousePressed[GLFW_MOUSE_BUTTON_LAST], mouseReleased[GLFW_MOUSE_BUTTON_LAST];

            /* Keyboard status. */
            bool keyDown[GLFW_KEY_LAST], keyPressed[GLFW_KEY_LAST], keyReleased[GLFW_KEY_LAST];
            char charPressed;
            
            /* Cursors. */
            GLFWcursor* cursorArrow;
            GLFWcursor* cursorHandpoint;
            GLFWcursor* cursorBeam;
            GLFWcursor* currentCursor;

            GLFWwindow* handle;
            std::function<void()> loopEventFunc, resizeEventFunc;
            std::function<void()> mouseEventFunc;
            std::function<void()> keyEventFunc;
            Mat4x4 ortho;
    };

}