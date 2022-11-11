#pragma once

#include "lib.h"
#include "Scene.h"
#include "GLProgram.h"

class Window
{
private:
    int width;
    int height;
    float ratio;
    unsigned int updateDelay;
    GLProgram *glProgram;
    Scene *scene;

public:
    Window(GLProgram *glProgram, Scene *scene, int width, int height);
    void init(int argc, char *argv[], char *title, int initX, int initY,
              void (*drawCallback)(), void (*updateCallback)(int), unsigned int updateDelay, void (*reshapeCallback)(int, int));
    void loop();
    void drawScene();
    void reshape(int w, int h);

    int getWidth();
    int getHeight();

    unsigned int getUpdateDelay();
};
