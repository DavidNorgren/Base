
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
    
    glProgram = glCreateProgram();
    glGenBuffers(1, &glVbo);
    
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
    glAttachShader(glProgram, vs);

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
    glAttachShader(glProgram, fs);

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
    glLinkProgram(glProgram);
    
    cout << "Shader compiled" << endl;
}

void Base::Shader::select()
{
    glUseProgram(glProgram);
}

void Base::Shader::render2D(Mat4f matrix, Vertex2Di* vertexData, int vertices, GLuint glTexture, Color color, GLenum mode)
{
    // Select shader program, get uniforms
    GLint aPos      = glGetAttribLocation(glProgram, "aPos");
    GLint aTexCoord = glGetAttribLocation(glProgram, "aTexCoord");
    GLint uMat      = glGetUniformLocation(glProgram, "uMat");
    GLint uSampler  = glGetUniformLocation(glProgram, "uSampler");
    GLint uColor    = glGetUniformLocation(glProgram, "uColor");

    // Select shader's VBO to send into the shader
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(Vertex2Di), &vertexData[0], GL_DYNAMIC_DRAW);

    // Pass buffers
    glEnableVertexAttribArray(aPos);
    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aPos,      2, GL_INT,   GL_FALSE, sizeof(Vertex2Di), 0);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2Di), (void*)sizeof(Vec2f));
    
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

void Base::Shader::render3D(Base::Mat4f matrix, GLuint vbo, int vertices, GLuint ibo, int indices, GLuint glTexture)
{
    // Select shader program, get uniforms
    GLint aPos      = glGetAttribLocation(glProgram, "aPos");
    GLint aNormal   = glGetAttribLocation(glProgram, "aNormal");
    GLint aTexCoord = glGetAttribLocation(glProgram, "aTexCoord");
    GLint uMat      = glGetUniformLocation(glProgram, "uMat");
    GLint uSampler  = glGetUniformLocation(glProgram, "uSampler");
    GLint uColor    = glGetUniformLocation(glProgram, "uColor");

    // Select shader's VBO to send into the shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Pass buffers
    glEnableVertexAttribArray(aPos);
    glEnableVertexAttribArray(aNormal);
    glEnableVertexAttribArray(aTexCoord);
    glVertexAttribPointer(aPos,      3, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), 0);
    glVertexAttribPointer(aNormal,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), (void*)sizeof(Vec3f));
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3Df), (void*)(sizeof(Vec3f) + sizeof(Vec3f)));

    // Send in matrix
    glUniformMatrix4fv(uMat, 1, GL_FALSE, matrix.elem);

    // Send in color
    Color color = { 1.f };
    glUniform4fv(uColor, 1, (float*)&color);

    // Send in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glUniform1i(uSampler, 0);

    // Draw all triangles
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

    // Reset
    glDisableVertexAttribArray(aPos);
    glDisableVertexAttribArray(aNormal);
    glDisableVertexAttribArray(aTexCoord);
    glBindTexture(GL_TEXTURE_2D, 0);
}