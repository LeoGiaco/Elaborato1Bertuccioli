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

vector<vec3> vertices;
vector<vec4> colors;

Shape *s1, *s2, *s3, *s4, *s5, *c1, *c2;
ComplexShape *cs;

float t = 0;

void drawCallback()
{
    w.drawScene();
}

void updateCallback(int v)
{
    t += DELTA_T;
    if (t > 360)
        t -= 360;

    cs->setRotationAroundAnchor(radians(10 * sin(2 * t)));

    glutTimerFunc(w.getUpdateDelay(), updateCallback, 0);
    glutPostRedisplay();
}

void reshapeCallback(int width, int height)
{
    w.reshape(width, height);
}

void createShapes()
{
#pragma region bruh
// vertices.push_back(vec3(1.0f, -1.0f, 1.0f));
// vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
// vertices.push_back(vec3(1.0f, 1.0f, 1.0f));
// vertices.push_back(vec3(-1.0f, 1.0f, 1.0f));
// vertices.push_back(vec3(-1.0f, -1.0f, 1.0f));

// colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
// colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
// colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
// colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
// colors.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
// colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));

// s = new Shape(&program, vertices, colors, GL_TRIANGLES);
// s->setAnchorPosition(400, 400);
// s->setScale(100);
// scene.addShape(s);

// s2 = Shape::circle(&program, 6, vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
// s2->setAnchorPosition(400, 400);
// s2->setPosition(-300, 0);
// s2->setScale(100);
// s2->setRotation(radians(180.0f));
// scene.addShape(s2);

// c = Shape::circle(&program, 30, vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
// c->setAnchorPosition(400, 400);
// c->setPosition(300, 0);
// c->setScale(90);
// scene.addShape(c);
#pragma endregion
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

    s1 = Shape::HermiteCurve(&program, 4, samples, derivs, vec4(1, 0, 0, 1), true, vec3(0, 0.2f, 0), vec4(1, 1, 0, 1));
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

    s2 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
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

    s3 = Shape::HermiteCurve(&program, 4, samples, derivs, vec4(1, 1, 1, 1), true, vec3(0.6f, 0, 0), vec4(0.8, 0.8, 0.8, 1));
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

    s4 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s4->setScale(50);
    s4->setPosition(40, 40);
    //////////
    ////////// hole
    s5 = Shape::circle(&program, 12, vec4(0, 0, 0, 1), vec4(0, 0, 0, 1));
    s5->setScale(10);
    s5->setPosition(40, 40);
    //////////

    c1 = Shape::circle(&program, 4, vec4(0, 0.3f, 0.75f, 0.8f), vec4(0, 0.3f, 0.75f, 0.8f));
    c1->rotate(radians(45.0f));
    c1->setPosition(w.getWidth() / 2.0f, 0);
    c1->setScale(w.getWidth() / 2.0f * 1.45f);

    cs = new ComplexShape(&program);
    cs->addShape(s2);
    cs->addShape(s3);
    cs->addShape(s1);
    cs->addShape(s5);
    cs->addShape(s4);
    cs->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    scene.addShape(cs);
    scene.addShape(c1);
}

int main(int argc, char *argv[])
{
    w.init(argc, argv, title, 100, 100, drawCallback, updateCallback, 16, reshapeCallback);
    GLProgramInstance *inst = program.createProgram("default", vshader, fshader);
    program.useProgram(inst);
    cout << glGetError() << endl;

    glClearColor(0, 0, 0.2f, 1.0f);

    createShapes();

    w.loop();
}