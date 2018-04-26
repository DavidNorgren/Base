#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "resource/resource.hpp"
#include "util/data/vertex2d.hpp"
#include "util/data/vertex3d.hpp"
#include "util/data/mat4.hpp"
#include "render/color.hpp"


namespace Base
{
    struct ShaderException : public ResourceLoadException
    {
        ShaderException(const string& message) : ResourceLoadException(message) {};
    };

    class Shader : public Resource
    {
      public:
        /* Loads shader code from a file and compiles it.
           The different shader types are separated in the file
           with comments. */
        Shader(const string& filename, function<void(GLuint)> setup = nullptr); 
        Shader(const Data& data,       function<void(GLuint)> setup = nullptr);

        /* Selects the shader for usage */
        EXPORT void select();

        /* Renders a 2D graphic using a projection matrix and buffers for vertex data. */
        EXPORT void render2D(Mat4f matrix, Vertex2Di* vertexData, int vertices, GLuint glTexture, Color color = { 1.f }, GLenum mode = GL_TRIANGLES);

        /* Renders a 3D mesh using a transformation matrix and buffers for vertex and index data. */
        EXPORT void render3D(Mat4f matrix, GLuint vbo, int vertices, GLuint ibo, int indices, GLuint glTexture);

        bool reload(const string& filename) override;
      private:
        void load(const string& source);

        string name;
        uint glProgram, glVbo;
        function<void(uint)> setup;
        bool isLoaded;
    };
}