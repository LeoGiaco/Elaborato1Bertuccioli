#include <iostream>
#include "./core/lib.h"
#include "./core/Window.h"
#include "./core/GLProgram.h"
#include "./core/Scene.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define DELTA_T 16.0f / 1000

char *vshader = (char *)"shaders/vertexShader.glsl";
char *fshader = (char *)"shaders/fragmentShader.glsl";
char *title = (char *)"Prova";

GLProgram program;
Scene scene;
Window w(&program, &scene, WINDOW_WIDTH, WINDOW_HEIGHT, ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT));

mat4 Projection;

Shape *sea;
ComplexShape *boat;
Shape *enemy1, *enemy2, *enemy3, *activeEnemy;
float t = 0;

float reload = 0;
int moving = 0, dir = 1;

Shape *chooseRandomEnemy()
{
    return enemy1; // TODO: implement.
}

void fire()
{
    reload = 2;
}

void drawCallback()
{
    w.drawScene();
}

void updateCallback(int v)
{
    if (reload > 0)
        reload -= DELTA_T;

    t += DELTA_T;
    if (t > 360)
        t -= 360;

    boat->setRotationAroundAnchor(radians(10 * sin(2 * t)));
    boat->shiftAnchorX(moving * 80 * DELTA_T);

    glutTimerFunc(w.getUpdateDelay(), updateCallback, 0);
    glutPostRedisplay();
}

void reshapeCallback(int width, int height)
{
    w.reshape(width, height);
}

void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
    if (key == ' ')
    {
        fire();
    }
}

void keyboardCallbackSp(int key, int mouseX, int mouseY)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        moving = -1;
        dir = -1;
        boat->setScaleRelativeToAnchor(-1, 1);
        break;
    case GLUT_KEY_RIGHT:
        moving = 1;
        dir = 1;
        boat->setScaleRelativeToAnchor(1, 1);
        break;
    default:
        break;
    }
}

void keyboardCallbackUpSp(int key, int mouseX, int mouseY)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        if (moving < 0) // Proofed against pressing both keys and releasing the first.
            moving = 0;
        break;
    case GLUT_KEY_RIGHT:
        if (moving > 0)
            moving = 0;
        break;
    default:
        break;
    }
}

void createBoat(ComplexShape **boat)
{
    ////////// boat
    vector<vec3> samples;
    vector<vec2> derivs;

    samples.push_back(vec3(1.05f, 0.665f, 0));
    samples.push_back(vec3(0.334f, 0.6f, 0));
    samples.push_back(vec3(0, 0.6f, 0));
    samples.push_back(vec3(-0.346f, 0.61f, 0));
    samples.push_back(vec3(-0.715f, 0.63f, 0));
    samples.push_back(vec3(-0.67f, 0.1f, 0));
    samples.push_back(vec3(0.13f, -0.02f, 0));
    samples.push_back(vec3(0.82f, 0.1f, 0));

    derivs.push_back(vec2(-0.005f, -0.237f));
    derivs.push_back(vec2(-0.119f, 0.0017f));
    derivs.push_back(vec2(-0.15f, 0.0013f));
    derivs.push_back(vec2(0.094f, 0.0037f));
    derivs.push_back(vec2(0, 0.0407f));
    derivs.push_back(vec2(0.448f, -0.43f));
    derivs.push_back(vec2(0.365f, 0.0273f));
    derivs.push_back(vec2(0.7267f, 0.3667f));

    Shape *s1 = Shape::HermiteCurve(&program, 4, samples, derivs, vec4(1, 0, 0, 1), true, vec3(0, 0.2f, 0), vec4(1, 1, 0, 1));
    s1->setScale(100);
    //////////
    ////////// mainmast
    vector<vec3> verticesRect;
    vector<vec4> colorsRect;

    verticesRect.push_back(vec3(0, 0, 0));
    verticesRect.push_back(vec3(0.1f, 0.8f, 0));
    verticesRect.push_back(vec3(-0.1f, 0.8f, 0));
    verticesRect.push_back(vec3(-0.1f, -0.8f, 0));
    verticesRect.push_back(vec3(0.1f, -0.8f, 0));
    verticesRect.push_back(vec3(0.1f, 0.8f, 0));

    for (int i = 0; i < verticesRect.size(); i++)
    {
        colorsRect.push_back(vec4(0.7f, 0.7f, 0.7f, 1));
    }

    Shape *s2 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s2->setScale(80);
    s2->setPosition(0, 110);
    //////////
    ////////// sail
    samples.clear();
    derivs.clear();

    samples.push_back(vec3(0.81f, 0.31f, 0));
    samples.push_back(vec3(0.01f, 0.47f, 0));
    samples.push_back(vec3(-0.9f, 0.5f, 0));
    samples.push_back(vec3(-0.72f, 0.29f, 0));
    samples.push_back(vec3(-0.72f, -0.25f, 0));
    samples.push_back(vec3(-0.874f, -0.506f, 0));
    samples.push_back(vec3(0, -0.53f, 0));
    samples.push_back(vec3(0.81f, -0.37f, 0));

    derivs.push_back(vec2(-0.742f, 0.585f));
    derivs.push_back(vec2(-0.5426f, 0.0174f));
    derivs.push_back(vec2(-0.268f, 0.011f));
    derivs.push_back(vec2(-0.005f, -0.3315f));
    derivs.push_back(vec2(0.002f, -0.215f));
    derivs.push_back(vec2(-0.5048f, -0.05f));
    derivs.push_back(vec2(0.6037f, -0.002f));
    derivs.push_back(vec2(0.7407f, 0.6059f));

    Shape *s3 = Shape::HermiteCurve(&program, 4, samples, derivs, vec4(1, 1, 1, 1), true, vec3(0.6f, 0, 0), vec4(0.8, 0.8, 0.8, 1));
    s3->setScale(70);
    s3->setPosition(0, 130);
    //////////
    ////////// cannon
    verticesRect.clear();
    colorsRect.clear();

    verticesRect.push_back(vec3(0.4f, 0.05f, 0));
    verticesRect.push_back(vec3(0.8f, 0.1f, 0));
    verticesRect.push_back(vec3(0, 0.1f, 0));
    verticesRect.push_back(vec3(0, -0.1f, 0));
    verticesRect.push_back(vec3(0.8f, -0.1f, 0));
    verticesRect.push_back(vec3(0.8f, 0.1f, 0));

    for (int i = 0; i < verticesRect.size(); i++)
    {
        colorsRect.push_back(vec4(0.3f, 0.3f, 0.3f, 1));
    }

    Shape *s4 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s4->setScale(50);
    s4->setPosition(40, 40);
    //////////
    ////////// hole
    Shape *s5 = Shape::circle(&program, 12, vec4(0, 0, 0, 1), vec4(0, 0, 0, 1));
    s5->setScale(10);
    s5->setPosition(40, 40);
    //////////

    (*boat) = new ComplexShape(&program);
    (*boat)->addShape(s2);
    (*boat)->addShape(s3);
    (*boat)->addShape(s1);
    (*boat)->addShape(s5);
    (*boat)->addShape(s4);
    (*boat)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
}

void createEnemies(Shape **e1, Shape **e2, Shape **e3)
{
}

void createSea(Shape **sea)
{
    (*sea) = Shape::circle(&program, 4, vec4(0, 0.3f, 0.75f, 0.8f), vec4(0, 0.3f, 0.75f, 0.8f)); // Will become a curve.
    (*sea)->rotate(radians(45.0f));
    (*sea)->setPosition(w.getWidth() / 2.0f, 0);
    (*sea)->setScale(w.getWidth() / 2.0f * 1.45f);
}

void createShapes()
{
    createBoat(&boat);
    createEnemies(&enemy1, &enemy2, &enemy3);
    createSea(&sea);

    scene.addShape(boat);
    scene.addShape(sea);
}

int main(int argc, char *argv[])
{
    w.init(argc, argv, title, 100, 100, drawCallback, updateCallback, 16, reshapeCallback);
    GLProgramInstance *inst = program.createProgram("default", vshader, fshader);
    program.useProgram(inst);
    cout << glGetError() << endl;

    glClearColor(0, 0, 0.2f, 1.0f);

    createShapes();

    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(keyboardCallbackSp);
    glutSpecialUpFunc(keyboardCallbackUpSp);
    glutIgnoreKeyRepeat(1);

    w.loop();
}