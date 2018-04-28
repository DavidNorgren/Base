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
        Shader() {};

        /* Sets the function to call for each object using the shader. */
        EXPORT void setSetupFunc(function<void(GLuint)> setup);

        /* Selects the shader for usage */
        EXPORT void select() const;

        /* Renders a 2D graphic using a projection matrix and buffers for vertex data. */
        EXPORT void render2D(const Mat4f& matrix, Vertex2Di* vertexData, int vertices, GLuint glTexture, const Color& color = { 1.f }, GLenum mode = GL_TRIANGLES) const;

        /* Renders a 3D mesh using a transformation matrix and buffers for vertex and index data. */
        EXPORT void render3D(const Mat4f& matrix, GLuint vbo, int vertices, GLuint ibo, int indices, GLuint glTexture) const;

      private:
        void load(const FilePath& file) override; 
        void load(const FileData& data) override;
        void load(const string& code);
        bool reload(const FilePath& file) override;

        string name;
        uint glProgram, glVbo;
        function<void(uint)> setup;
    };
}