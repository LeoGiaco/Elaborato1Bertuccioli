#pragma once

#include <string>
#include "lib.h"

class GLProgramInstance
{
private:
    GLuint id;

public:
    GLProgramInstance(GLuint id);
    void useProgram();
    GLuint getID();
    GLint getUniformLocation(char *varName);
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
    void useProgram(GLProgramInstance *instance);
    GLProgramInstance *getUsedProgram();
};
