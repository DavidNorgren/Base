#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "resource/resourcehandler.hpp"
#include "resource/font.hpp"
#include "resource/image.hpp"
#include "resource/shader.hpp"
#include "render/color.hpp"
#include "scene/scene.hpp"
#include "window/window.hpp"
#include "util/mathfunc.hpp"


namespace Base
{
    /* Handles an OpenGL application. Used as a parent for the project class. */
    class AppHandler
    {
      protected:
        /* Setup OpenGL and load resources. */
        EXPORT AppHandler();

        /* Called by the main window. Implemented by the child. */
        EXPORT virtual void loopEvent() = 0;
        EXPORT void mouseEvent() {};
        EXPORT void keyEvent() {};
        EXPORT void resizeEvent() {};

        /* Opens the window and starts the window loop. */
        EXPORT void launch();

      public:
        // Window
        Window* mainWindow;

        // Resources
        ResourceHandler* resHandler;
        Font* drawingFont;
        Shader* drawingShader;
        float drawingAlpha = 1.f;
        ScreenArea drawingArea;
        Image* solidColor;

        // Scene
        Scene* currentScene;
    };
}

extern Base::AppHandler* appHandler;