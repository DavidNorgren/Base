#include "common.hpp"
#include "apphandler.hpp"
#include "render/colors.hpp"


// Reference to appHandler instance
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

    cout << "GLEW version: "      << glewGetString(GLEW_VERSION)              << endl;
    cout << "OpenGL version: "    << glGetString(GL_VERSION)                  << endl;
    cout << "OpenGL SL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "OpenGL vendor: "     << glGetString(GL_VENDOR)                   << endl;
    cout << endl;
    
    // Culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Transparent blending
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    // Texture settings
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

EXPORT Base::AppHandler::AppHandler(char resData[], uint resSize) : AppHandler()
{
    resHandler = new ResourceHandler(resData, resSize);
    createResources();
}

EXPORT Base::AppHandler::AppHandler(const DirectoryPath& resDir) : AppHandler()
{
    resHandler = new ResourceHandler(resDir);
    createResources();
}

void Base::AppHandler::createResources()
{
    drawingFont   = (Font*)resHandler->get("fonts/opensans.ttf");
    drawingShader = (Shader*)resHandler->get("shaders/texture.glsl");
}

EXPORT void Base::AppHandler::launch()
{
    mainWindow->open(std::bind(&AppHandler::loopEvent, this),
                     std::bind(&AppHandler::mouseEvent, this),
                     std::bind(&AppHandler::keyEvent, this),
                     std::bind(&AppHandler::resizeEvent, this));
}