
#include "common.hpp"
#include "render/shader.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"


Base::Shader::Shader(string filename, function<void(GLuint)> setup)
{
    name = filename;
    this->setup = setup;
    load(fileGetContents(filename));
}

Base::Shader::Shader(File* file, function<void(GLuint)> setup)
{
    name = file->name;
    this->setup = setup;
    load(string(file->rawData, file->size));
}

void Base::Shader::load(string source)
{
    cout << "Loading " << name << "..." << endl;
    
    program = glCreateProgram();
    glGenBuffers(1, &vbo);
    
    // Split the source code by the comments into a Vertex and Fragment shader
    List<string> sourceSplit = stringSplit(stringReplace(source, "\r\n", "\n"), "// Fragment\n");
    string vertexSource = sourceSplit[0];
    string fragmentSource = sourceSplit[1];
    
    // Vertex shader setup
    GLint isCompiled;
    GLint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vsSource = vertexSource.c_str();
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    glAttachShader(program, vs);

    // Vertex shader error handling
    if (!isCompiled)
    {
        GLint errorLength;
        string error;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &errorLength);
        error.resize(errorLength);
        glGetShaderInfoLog(vs, errorLength, 0, &error[0]);
        cout << "VERTEX SHADER COMPILATION ERROR!" << endl << error << " in " << name << endl << endl;
    }
    
    // Fragment shader setup
    GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fsSource = fragmentSource.c_str();
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    glAttachShader(program, fs);

    // Fragment shader error handling
    if (!isCompiled)
    {
        GLint errorLength;
        string error;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &errorLength);
        error.resize(errorLength);
        glGetShaderInfoLog(fs, errorLength, 0, &error[0]);
        cout << "FRAGMENT SHADER COMPILATION ERROR!" << endl << error << " in " << name << endl << endl;
    }

    // Link shader program
    glLinkProgram(program);
    
    cout << "Shader compiled" << endl;
}

void Base::Shader::select()
{
    glUseProgram(program);
}

void Base::Shader::render2D(Mat4x4f matrix, Vec3f* posData, Vec2f* texCoordData, int vertices, GLuint glTexture, Color color, GLenum mode)
{
    // Select shader program, get uniforms
    GLint aPos = glGetAttribLocation(program, "aPos");
    GLint aTexCoord = glGetAttribLocation(program, "aTexCoord");
    GLint uMat = glGetUniformLocation(program, "uMat");
    GLint uSampler = glGetUniformLocation(program, "uSampler");
    GLint uColor = glGetUniformLocation(program, "uColor");

    // Select shader's VBO to send into the shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Bind buffers with data
    uint sizePositions = vertices * sizeof(Vec3f);
    uint sizeTexCoords = vertices * sizeof(Vec2f);
    glBufferData(GL_ARRAY_BUFFER, sizePositions + sizeTexCoords, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizePositions, posData);
    glBufferSubData(GL_ARRAY_BUFFER, sizePositions, sizeTexCoords, texCoordData);

    // Pass buffers
    glEnableVertexAttribArray(aPos);
    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizePositions);

    // Send in matrix
    glUniformMatrix4fv(uMat, 1, GL_FALSE, matrix.elem);

    // Send in color
    glUniform4fv(uColor, 1, (float*)&color);

    // Send in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glUniform1i(uSampler, 0);

    // Draw all triangles
    glDrawArrays(mode, 0, vertices);

    // Reset
    glDisableVertexAttribArray(aPos);
    glDisableVertexAttribArray(aTexCoord);
    glBindTexture(GL_TEXTURE_2D, 0);
}