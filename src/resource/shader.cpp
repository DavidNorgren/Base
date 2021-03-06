#include "common.hpp"
#include "resource/shader.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"
#include "apphandler.hpp"


EXPORT void Base::Shader::setSetupFunc(ShaderSetup setup)
{
    this->setup = setup;
}

EXPORT void Base::Shader::select() const
{
    if (isLoaded)
        glUseProgram(glProgram);
}

EXPORT void Base::Shader::render2D(const Mat4f& matMVP, List<Vertex2Di> vertexData, Image* image, const Color& color, GLenum mode) const
{
    if (!isLoaded)
        return;

    // Select shader program, get uniforms
    GLint aPos      = glGetAttribLocation(glProgram, "aPos");
    GLint aTexCoord = glGetAttribLocation(glProgram, "aTexCoord");
    GLint uMatMVP   = glGetUniformLocation(glProgram, "uMatMVP");
    GLint uSampler  = glGetUniformLocation(glProgram, "uSampler");
    GLint uColor    = glGetUniformLocation(glProgram, "uColor");

    // Select shader's VBO to send into the shader
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex2Di), &vertexData[0], GL_DYNAMIC_DRAW);

    // Pass buffers
    glEnableVertexAttribArray(aPos);
    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aPos,      2, GL_INT,   GL_FALSE, sizeof(Vertex2Di), 0);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2Di), (void*)sizeof(Vec2f));
    
    // Send in matrix
    glUniformMatrix4fv(uMatMVP, 1, GL_FALSE, matMVP.e);

    // Send in color
    glUniform4fv(uColor, 1, (float*)&color);

    // Send in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, (image ? image->getGlTexture() : Image::getSingleColor(Colors::WHITE)->getGlTexture()));
    glUniform1i(uSampler, 0);

    // Call setup
    if (setup)
        setup(glProgram, Mat4f::identity());

    // Draw all triangles
    glDrawArrays(mode, 0, vertexData.size());

    // Reset
    glDisableVertexAttribArray(aPos);
    glDisableVertexAttribArray(aTexCoord);
    glBindTexture(GL_TEXTURE_2D, 0);
}

EXPORT void Base::Shader::render3D(const Mat4f& matM, const Mat4f& matVP, Base::TriangleMesh* mesh, Base::Material* material) const
{
    if (!isLoaded)
        return;

    // Select shader program, get uniforms
    GLint aPos      = glGetAttribLocation(glProgram, "aPos");
    GLint aTexCoord = glGetAttribLocation(glProgram, "aTexCoord");
    GLint aNormal   = glGetAttribLocation(glProgram, "aNormal");
    GLint uMatM     = glGetUniformLocation(glProgram, "uMatM");
    GLint uMatMVP   = glGetUniformLocation(glProgram, "uMatMVP");
    GLint uSampler  = glGetUniformLocation(glProgram, "uSampler");
    GLint uColor    = glGetUniformLocation(glProgram, "uColor");

    // Select shader's VBO to send into the shader
    glBindBuffer(GL_ARRAY_BUFFER, mesh->glVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->glIbo);

    // Pass buffers
    glEnableVertexAttribArray(aPos);
    glEnableVertexAttribArray(aTexCoord);
    glEnableVertexAttribArray(aNormal);
    glVertexAttribPointer(aPos,      3, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), 0);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), (void*)sizeof(Vec3f));
    glVertexAttribPointer(aNormal,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), (void*)(sizeof(Vec2f) + sizeof(Vec3f)));

    // Send in matrices
    glUniformMatrix4fv(uMatM,   1, GL_FALSE, matM.e);
    glUniformMatrix4fv(uMatMVP, 1, GL_FALSE, (matVP * matM).e);

    // Send in color
    Color color = { 1.f };
    glUniform4fv(uColor, 1, (float*)&color);

    // Send in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, (material ? material->getTexture()->getGlTexture() : Image::getSingleColor(Colors::WHITE)->getGlTexture()));
    glUniform1i(uSampler, 0);

    // Call setup
    if (setup)
        setup(glProgram, matM);

    // Draw all triangles
    glEnable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, mesh->indexData.size(), GL_UNSIGNED_INT, 0);

    if (appHandler->debugShowLines)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(4);
        glUniform4fv(uColor, 1, (float*)&Colors::BLACK);
        glDrawElements(GL_TRIANGLES, mesh->indexData.size(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glDisable(GL_DEPTH_TEST);

    // Reset
    glDisableVertexAttribArray(aPos);
    glDisableVertexAttribArray(aTexCoord);
    glDisableVertexAttribArray(aNormal);
}

void Base::Shader::load(const FilePath& file)
{
    glGenBuffers(1, &glVbo);
    load(fileGetText(file));
}

void Base::Shader::load(const FileData& data)
{
    glGenBuffers(1, &glVbo);
    load(string(&data[0], data.size()));
}

void Base::Shader::load(const string& code)
{
    if (code.length() == 0)
        throw ShaderException("No shader code submitted.");
    
    glProgram = glCreateProgram();
    isLoaded = false;

    // Split the source code by the comments into a Vertex and Fragment shader
    List<string> sourceSplit = stringSplit(stringReplace(code, "\r\n", "\n"), "// Fragment\n");
    string vertexSource = sourceSplit[0];
    string fragmentSource = sourceSplit[1];

    // Show the correct line number in fragment error messages
    fragmentSource = stringRepeat("\n", stringGetCount(vertexSource, "\n")) + fragmentSource;
    
    // Vertex shader setup
    GLint isCompiled;
    GLint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vsSource = vertexSource.c_str();
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    glAttachShader(glProgram, vs);

    // Vertex shader error handling
    if (!isCompiled)
    {
        GLint errorLength;
        string error;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &errorLength);
        error.resize(errorLength);
        glGetShaderInfoLog(vs, errorLength, 0, &error[0]);
        glDeleteShader(vs);
        glDeleteProgram(glProgram);
        glProgram = 0;
        throw ShaderException("Vertex shader compilation error:\n\t" + stringReplace(error, "\n", "\n\t"));
    }
    
    // Fragment shader setup
    GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fsSource = fragmentSource.c_str();
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    glAttachShader(glProgram, fs);

    // Fragment shader error handling
    if (!isCompiled)
    {
        GLint errorLength;
        string error;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &errorLength);
        error.resize(errorLength);
        glGetShaderInfoLog(fs, errorLength, 0, &error[0]);
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(glProgram);
        glProgram = 0;
        throw ShaderException("Fragment shader compilation error:\n\t" + stringReplace(error, "\n", "\n\t"));
    }

    // Link shader program
    glLinkProgram(glProgram);
    glDetachShader(glProgram, vs);
    glDetachShader(glProgram, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Base::Shader::cleanUp()
{
    if (glVbo)
        glDeleteBuffers(1, &glVbo);
    if (glProgram)
        glDeleteProgram(glProgram);
}