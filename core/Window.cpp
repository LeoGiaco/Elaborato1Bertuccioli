#include "Window.h"

Window::Window(GLProgram *glProgram, Scene *scene, int width, int height, mat4 proj)
    : glProgram(glProgram), scene(scene), width(width), height(height), projection(proj) {}

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

    GLint projId = glProgram->getUsedProgram()->getUniformLocation((char *)"projection");
    glUniformMatrix4fv(projId, 1, GL_FALSE, value_ptr(projection));

    scene->draw();

    glutSwapBuffers();
}

void Window::reshape(int w, int h)
{
    GLint resId = glProgram->getUsedProgram()->getUniformLocation((char *)"resolution");

    width = w;
    height = h;

    float AspectRatio_mondo = 1.0f; // As class field.
    if (AspectRatio_mondo > w / h)
    {
        glViewport(0, 0, w, w / AspectRatio_mondo);
        glUniform2f(resId, w, w / AspectRatio_mondo);
    }
    else
    {
        glViewport(0, 0, h * AspectRatio_mondo, h);
        glUniform2f(resId, h * AspectRatio_mondo, h);
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

mat4 Window::getProjectionMatrix()
{
    return projection;
}
