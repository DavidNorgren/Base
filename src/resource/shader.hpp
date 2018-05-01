#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

#include "util/data/vertex2d.hpp"
#include "util/data/mat4.hpp"
#include "render/color.hpp"
#include "render/image.hpp"
#include "resource/resource.hpp"
#include "scene/trianglemesh.hpp"
#include "scene/material.hpp"


namespace Base
{
    using ShaderSetup = function<void(GLuint, const Mat4f&)>;

    struct ShaderException : public ResourceLoadException
    {
        ShaderException(const string& message) : ResourceLoadException(message) {};
    };

    class TriangleMesh;
    class Shader : public Resource
    {
      public:
        Shader() {};

        /* Sets the function to call for each object using the shader. */
        EXPORT void setSetupFunc(ShaderSetup setup);

        /* Selects the shader for usage */
        EXPORT void select() const;

        /* Renders a 2D graphic using a projection matrix and a buffer for vertex data. */
        EXPORT void render2D(const Mat4f& matMVP, List<Vertex2Di> vertexData, Image* texture = nullptr, const Color& color = { 1.f }, GLenum mode = GL_TRIANGLES) const;

        /* Renders a 3D mesh using a transformation matrix and an optional material. */
        EXPORT void render3D(const Mat4f& matM, const Mat4f& matVP, TriangleMesh* mesh, Material* material = nullptr) const;

      private:
        void load(const FilePath& file) override; 
        void load(const FileData& data) override;
        void load(const string& code);
        void cleanUp() override;

        string name;
        GLuint glProgram, glVbo;
        ShaderSetup setup;
    };
}