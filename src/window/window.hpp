#pragma once

#include "GLFW/glfw3.h"

#include "util/math/vec2.hpp"
#include "util/math/mat4x4.hpp"
#include "util/math/mathfunc.hpp"
#include "util/stringfunc.hpp"
#include "window/screenpos.hpp"
#include "render/color.hpp"


namespace Base
{
    /* Opens and manages a resizable window. */
    class Window
    {
        public:
            /* Creates the window and OpenGL context. */
            EXPORT Window();

            /* Opens the window. */
            EXPORT void open(function<void()> loopEventFunc,
                             function<void()> mouseEventFunc = nullptr,
                             function<void()> keyEventFunc = nullptr,
                             function<void()> resizeEventFunc = nullptr);

            /* Sets the title of the window caption. */
            EXPORT void setTitle(string title);
            
            /* Sets the mouse cursor. */
            EXPORT void setCursor(GLFWcursor* cursor);

            /* Maximizes the window. */
            EXPORT void maximize();

            /* Window size. */
            int width, height;
            float ratio;

            /* Background. */
            Color backgroundColor = COLOR_BLACK;

            /* Window render FPS. */
            int fps;

            /* Mouse position and button status. */
            ScreenPos mousePos, mousePosPrevious, mousePosClick;
            Vec2 mouseMove;
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
            Mat4x4 ortho;
    };
}