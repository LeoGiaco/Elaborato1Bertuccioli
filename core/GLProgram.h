#pragma once

#include <string>
#include "lib.h"

class GLProgramInstance
{
private:
    GLuint id;
    mat4 projection;
    vec2 resolution;

public:
    GLProgramInstance(GLuint id);
    void useProgram();
    void setProjectionMatrix(mat4 matrix);
    void setResolution(vec2 res);
    GLuint getID();
    GLint getUniformLocation(string varName);
};

class GLProgram
{
private:
    char *readShaderSource(const char *shaderFile);
    map<std::string, GLProgramInstance *> programs;
    GLProgramInstance *usedProgram;

public:
    GLProgramInstance *createProgram(std::string programName, char *vShaderName, char *fShaderName);
    GLProgramInstance *getProgram(std::string programName);
    size_t programsCount();
    map<string, GLProgramInstance *>::iterator getIterator();
    void useProgram(GLProgramInstance *instance);
    void useProgram(string programName);
    GLProgramInstance *getUsedProgram();
};
