#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/math/mathfunc.hpp"
#include "file/resourcehandler.hpp"
#include "render/color.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/shader.hpp"
#include "window/window.hpp"
#include "window/screenarea.hpp"
#include "scene/scene.hpp"

namespace Base
{
    /* Handles an OpenGL application. Used as a parent for the project class. */
    class AppHandler
    {
      protected:
        /* Setup OpenGL and load resources. */
        EXPORT AppHandler(void* resData = nullptr, uint resSize = 0);

        /* Called by the main window. Implemented by the child. */
        EXPORT virtual void loopEvent() = 0;
        EXPORT virtual void mouseEvent() = 0;
        EXPORT virtual void keyEvent() = 0;
        EXPORT virtual void resizeEvent() = 0;

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