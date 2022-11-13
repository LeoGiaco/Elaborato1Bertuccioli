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

    // Enables the shader program.
    void useProgram();

    // Sets the projection matrix for the shader program.
    void setProjectionMatrix(mat4 matrix);

    // Sets the resolution vector for the shader program.
    void setResolution(vec2 res);

    // Returns the ID of the shader program.
    GLuint getID();

    // Returns the ID for the given uniform variable in the shader program.
    GLint getUniformLocation(string varName);
};

class GLProgram
{
private:
    map<std::string, GLProgramInstance *> programs;
    GLProgramInstance *usedProgram;

    char *readShaderSource(const char *shaderFile);

public:
    // Creates a new shader program and returns it.
    GLProgramInstance *createProgram(std::string programName, char *vShaderName, char *fShaderName);

    // Returns the shader program with the given name.
    GLProgramInstance *getProgram(std::string programName);

    // Returns the number of created shader programs.
    size_t programsCount();

    // Returns an iterator for all shader programs.
    map<string, GLProgramInstance *>::iterator getIterator();

    // Enables the given shader program.
    void useProgram(GLProgramInstance *instance);

    // Enables the shader program with the given name.
    void useProgram(string programName);

    // Returns the shader program currently in use.
    GLProgramInstance *getUsedProgram();
};
