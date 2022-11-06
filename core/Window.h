#pragma once

#include "lib.h"
#include "Scene.h"
#include "GLProgram.h"

class Window
{
private:
    int width;
    int height;
    unsigned int updateDelay;
    mat4 projection;
    GLProgram *glProgram;
    Scene *scene;

public:
    Window(GLProgram *glProgram, Scene *scene, int width, int height, mat4 proj);
    void init(int argc, char *argv[], char *title, int initX, int initY,
              void (*drawCallback)(), void (*updateCallback)(int), unsigned int updateDelay, void (*reshapeCallback)(int, int));
    void loop();
    void drawScene();
    void reshape(int w, int h);

    int getWidth();
    int getHeight();
    mat4 getProjectionMatrix();

    unsigned int getUpdateDelay();
};
