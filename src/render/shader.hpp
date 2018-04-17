#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/math/vec2.hpp"
#include "util/math/vec3.hpp"
#include "util/math/mat4x4.hpp"
#include "file/file.hpp"
#include "render/color.hpp"


namespace Base
{
    class Shader
    {
        public:
            /*!
                * Loads shader code from a file and compiles it.
                * The different shader types are separated in the file
                * with comments.
                */
            Shader(string filename, function<void(GLuint)> setup = nullptr);
            Shader(File* file, function<void(GLuint)> setup = nullptr);

            /* Selects the shader for usage */

            /// <summary>Sorts the list to by the given column</summary>
            /// <returns>Documentation of return type</returns>
            void select();

            /*!
                *  @brief Renders a 2D graphic using the shader
                * 
                *  Renders a 2D graphic using a projection matrix and
                *  buffers for position and texture coordinate data.
                * 
                *  @param[in] matrix The transformation matrix to be applied.
                *  @param[in] posData Pointer to the buffer where position data is stored.
                *  @param[in] texCoordData Pointer to the buffer where texture coordinate data is stored.
                *  @param[in] vertices The amount of vertices in the buffer.
                *  @param[in] texture The OpenGL texture to use.
                *  @param[in] mode The OpenGL mode to use when rendering.
                */
            void render2D(Mat4x4 matrix, Vec3* posData, Vec2* texCoordData, int vertices, GLuint texture, Color color = { 1.f }, GLenum mode = GL_TRIANGLES);

        private:
            void load(string source);

            string name;
            uint program, vbo;
            function<void(uint)> setup;
    };
}