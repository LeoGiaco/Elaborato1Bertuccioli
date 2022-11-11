#include "Window.h"

Window::Window(GLProgram *glProgram, Scene *scene, int width, int height)
    : glProgram(glProgram), scene(scene), width(width), height(height), ratio(((float)width / height)) {}

void Window::init(int argc, char *argv[], char *title, int initX, int initY,
                  void (*drawCallback)(), void (*updateCallback)(int), unsigned int updateDelay, void (*reshapeCallback)(int, int))
{
    this->updateDelay = updateDelay;
    glutInit(&argc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(initX, initY);
    glutCreateWindow(title);

    glutDisplayFunc(drawCallback);
    glutReshapeFunc(reshapeCallback);
    glutTimerFunc(updateDelay, updateCallback, 0);

    glewExperimental = GL_TRUE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewInit();
}

void Window::loop()
{
    glutMainLoop();
}

void Window::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    scene->draw();

    glutSwapBuffers();
}

void Window::reshape(int w, int h)
{
    auto it = glProgram->getIterator();

    width = w;
    height = h;

    if (ratio > w / h)
    {
        glViewport(0, 0, w, w / ratio);
        for (size_t i = 0; i < glProgram->programsCount(); i++)
        {
            GLProgramInstance *inst = (*it).second;
            inst->setResolution(vec2((float)w, w / ratio));
        }
    }
    else
    {
        glViewport(0, 0, h * ratio, h);
        for (size_t i = 0; i < glProgram->programsCount(); i++)
        {
            GLProgramInstance *inst = (*it).second;
            inst->setResolution(vec2(h * ratio, (float)h));
        }
    }
}

unsigned int Window::getUpdateDelay()
{
    return updateDelay;
}

int Window::getWidth()
{
    return width;
}

int Window::getHeight()
{
    return height;
}