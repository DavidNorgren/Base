#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/data/vec2.hpp"
#include "util/data/vec3.hpp"
#include "util/data/mat4x4.hpp"
#include "file/file.hpp"
#include "render/color.hpp"


namespace Base
{
    class Shader
    {
      public:
        /* Loads shader code from a file and compiles it.
           The different shader types are separated in the file
           with comments. */
        Shader(string filename, function<void(GLuint)> setup = nullptr);
        Shader(File* file, function<void(GLuint)> setup = nullptr);

        /* Selects the shader for usage */
        void select();

        /* Renders a 2D graphic using a projection matrix and
           buffers for position and texture coordinate data. */
        void render2D(Mat4x4f matrix, Vec3f* posData, Vec2f* texCoordData, int vertices, GLuint glTexture, Color color = { 1.f }, GLenum mode = GL_TRIANGLES);

      private:
        void load(string source);

        string name;
        uint glProgram, glVbo;
        function<void(uint)> setup;
    };
}