#pragma once

#include "GLFW/glfw3.h"

#include "util/data/mat4.hpp"
#include "util/data/region2d.hpp"
#include "render/colors.hpp"
#include "render/renderfunc.hpp"


namespace Base
{
    /* The a cursor style. */
    enum class CursorStyle
    {
        ARROW,
        HANDPOINT,
        BEAM,
        WERESIZE,
        NSRESIZE
    };

    /* Opens and manages a resizable window. */
    class Window : public RenderTarget
    {
      public:
        EXPORT Window();

        EXPORT void  open(function<void()> loopEventFunc,
                          function<void()> mouseEventFunc = nullptr,
                          function<void()> keyEventFunc = nullptr,
                          function<void()> resizeEventFunc = nullptr);

        EXPORT void  maximize();

        /* Appearence */
        EXPORT void  setTitle(const string& title);
        EXPORT void  setCursor(CursorStyle cursor);

        /* Framerate */
        EXPORT void  setTargetFramerate(int fps) { targetFps = fps; }
        EXPORT float getFrameDelay() const;
        EXPORT int   getFps() const { return fps; }

        /* Mouse position and button status. */
        EXPORT ScreenPos getMousePos() const                { return mousePos; }
        EXPORT ScreenPos getMousePosPrevious() const        { return mousePosPrevious; }
        EXPORT ScreenPos getMousePosClick() const           { return mousePosClick; }
        EXPORT Vec2i     getMouseMove() const               { return mouseMove; }
        EXPORT bool      getMouseDown(int button) const     { return mouseDown[button]; }
        EXPORT bool      getMousePressed(int button) const  { return mousePressed[button]; }
        EXPORT bool      getMouseReleased(int button) const { return mouseReleased[button]; }
        EXPORT double    getMouseLastClickDuration() const  { return mouseLastClickDuration; }
        EXPORT Vec2f     getMouseScroll() const             { return mouseScroll; }
        EXPORT void      mouseClear();

        /* Keyboard status. */
        EXPORT bool getKeyDown(int key) const     { return keyDown[key]; }
        EXPORT bool getKeyPressed(int key) const  { return keyPressed[key]; }
        EXPORT bool getKeyReleased(int key) const { return keyReleased[key]; }
        EXPORT char getCharPressed() const        { return charPressed; }

        /* GLFW */
        void glfwMouseButtonCallback(int button, int action, int mods);
        void glfwScrollCallback(double x, double y);
        void glfwCursorPosCallback(double x, double y);
        void glfwKeyCallback(int key, int scancode, int action, int mods);
        void glfwCharModsCallback(uint codepoint, int mods);
        void glfwSizeCallback(int width, int height);

      private:
        function<void()> loopEventFunc;
        function<void()> resizeEventFunc;
        function<void()> mouseEventFunc;
        function<void()> keyEventFunc;
        int fps = 0;
        int targetFps = 60;

        ScreenPos mousePos, mousePosPrevious, mousePosClick;
        Vec2i mouseMove;
        bool mouseDown[GLFW_MOUSE_BUTTON_LAST];
        bool mousePressed[GLFW_MOUSE_BUTTON_LAST];
        bool mouseReleased[GLFW_MOUSE_BUTTON_LAST];
        double mouselastClickTime = 0;
        double mouseLastClickDuration = 0;
        Vec2f mouseScroll;

        bool keyDown[GLFW_KEY_LAST];
        bool keyPressed[GLFW_KEY_LAST];
        bool keyReleased[GLFW_KEY_LAST];
        char charPressed;
        
        GLFWwindow* handle;
        GLFWcursor* cursorArrow;
        GLFWcursor* cursorHandpoint;
        GLFWcursor* cursorBeam;
        GLFWcursor* cursorHResize;
        GLFWcursor* cursorVResize;
        GLFWcursor* currentCursor;
    };
}