#include <iostream>

#include "apphandler.hpp"


Base::AppHandler* appHandler;

Base::AppHandler::AppHandler()
{
    appHandler = this;
    
    // Initialize GLFW and create main window
    glfwInit();
    mainWindow = new Window();
    
    // Initialize GLEW
    glewExperimental = true;
    glewInit();

    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL SL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << std::endl;
    
    // Culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Transparent blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Texture settings
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Resources
    resourceHandler = new ResourceHandler();
    drawingFont = (Font*)resourceHandler->find(DRAWING_FONT)->loaded;
    drawingShader = (Shader*)resourceHandler->find(DRAWING_SHADER)->loaded;
    solidTexture = new Image(COLOR_WHITE, 1, 1);
}


void Base::AppHandler::launch()
{
    mainWindow->open(std::bind(&AppHandler::loopEvent, this),
                     std::bind(&AppHandler::mouseEvent, this, std::placeholders::_1),
                     std::bind(&AppHandler::keyEvent, this, std::placeholders::_1),
                     std::bind(&AppHandler::resizeEvent, this));
}