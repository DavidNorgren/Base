#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "resource/resourcehandler.hpp"
#include "file/directorypath.hpp"
#include "resource/font.hpp"
#include "resource/shader.hpp"
#include "render/color.hpp"
#include "render/image.hpp"
#include "window/window.hpp"
#include "util/mathfunc.hpp"
#include "scene/scene.hpp"


namespace Base
{
    /* Handles an OpenGL application. Used as a parent for the project class. */
    class AppHandler
    {
      protected:
        /* Setup the application and load resources from memory. */
        EXPORT AppHandler(char resData[], uint resSize);

        /* Setup the application and load resources dynamically. */
        EXPORT AppHandler(const DirectoryPath& resDir);

        /* Called by the main window. Implemented by the child. */
        EXPORT virtual void loopEvent() = 0;
        EXPORT virtual void mouseEvent() {};
        EXPORT virtual void keyEvent() {};
        EXPORT virtual void resizeEvent() {};

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
        
        bool debugShowLines = false;

      private:
        AppHandler();
        void createResources();
    };
}

extern Base::AppHandler* appHandler;