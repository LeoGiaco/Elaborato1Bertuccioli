#include <iostream>
#include "./core/lib.h"
#include "./core/Window.h"
#include "./core/GLProgram.h"
#include "./core/Scene.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define DELTA_T 16.0f / 1000

#define WAVES_ON_SCREEN 3
#define WAVES_POINTS 100
#define WAVE_WIDTH_NORM 2.0f / WAVES_ON_SCREEN
#define WAVE_WIDTH (float)WINDOW_WIDTH / WAVES_ON_SCREEN // WINDOW_WIDTH / 2.0f * WAVE_WIDTH_NORM
#define WAVE_MAX_HEIGHT_NORM 80.0f / WINDOW_HEIGHT
#define WAVE_MAX_HEIGHT WINDOW_HEIGHT *WAVE_MAX_HEIGHT_NORM

#define BOAT_FIXED_X WINDOW_WIDTH / 5.0f

#define MOD(f1, f2) ((f1) - (f2)*floorf((f1) / (f2)))

char *vshader = (char *)"shaders/vertexShader.glsl";
char *fshader = (char *)"shaders/fragmentShader.glsl";
char *title = (char *)"Prova";

GLProgram program;
Scene scene;
list<Shape *> projectiles;
Window w(&program, &scene, WINDOW_WIDTH, WINDOW_HEIGHT, ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT));

mat4 Projection;

Shape *sea;
ComplexShape *boat;
Shape *enemy1, *enemy2, *enemy3, *activeEnemy;
float t = 0;

float reload = 0;
int moving = 0, dir = 1;
float relBoatX = 0;

float waveRel = 0;

Shape *chooseRandomEnemy()
{
    return enemy1; // TODO: implement.
}

#pragma region wave
float getAngle(float xN)
{
    return radians(MOD(180.0f * (xN + 1) * WAVES_ON_SCREEN, 360.0f));
}

float getNormalizedWaveAngle(float xN)
{
    return asinf(WAVE_MAX_HEIGHT_NORM * sin(getAngle(xN)));
}

float getNormalizedWaveHeight(float xN)
{
    return sin(getNormalizedWaveAngle(xN));
}

float getWaveHeight(float x)
{
    // map [0, WINDOW_WIDTH] to [-1, 1]
    return WINDOW_HEIGHT / 2.0f + WINDOW_HEIGHT * getNormalizedWaveHeight(2 * x / WINDOW_WIDTH - 1.0f);
    // map output [-1, 1] to [0, WINDOW_HEIGHT]
}

float getWaveTangentAngle(float x)
{
    float y1 = getWaveHeight(x);
    float y2 = getWaveHeight(x + 1);

    return atan2(y2 - y1, 1); // Approximation of the angle of the wave.
}

#pragma endregion

void fire()
{
    if (reload <= 0)
    {
        reload = 0.1f;

        Shape *proj = Shape::circle(&program, 12, vec4(0, 0, 0, 1), vec4(0.3f, 0.3f, 0.3f, 0.3f));
        proj->setScale(15);
        proj->setAnchorPosition(boat->getAnchorPositionOf(3));
        proj->rotateAroundAnchor(boat->getAnchorAngleOf(3));

        scene.addShape(proj);
        projectiles.push_back(proj);
    }
}

#pragma region callbacks
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

    waveRel -= 30 * DELTA_T;
    relBoatX += moving * 200 * DELTA_T;

    boat->setRotationAroundAnchor(getWaveTangentAngle(BOAT_FIXED_X + relBoatX - waveRel));
    boat->setAnchorY(getWaveHeight(BOAT_FIXED_X + relBoatX - waveRel) - 10.0f);
    sea->setX(-MOD(relBoatX - waveRel, WAVE_WIDTH)); // The wave moves in the negative x

    vector<Shape *> removed;

    auto it = projectiles.begin();
    for (size_t i = 0; i < projectiles.size(); i++)
    {
        Shape *p = (*it);

        p->shiftX(250 * DELTA_T);

        if (p->getPosition()[0] >= WINDOW_WIDTH)
        {
            cout << "OUT " << t << endl;
            removed.push_back(p);
            p->setEnabled(false);
        }

        if (p->isColliding(enemy1))
        {
            cout << "HIT " << t << endl;
            removed.push_back(p);
            p->setEnabled(false);
        }

        ++it;
    }

    for (size_t i = 0; i < removed.size(); i++)
    {
        projectiles.remove(removed[i]);
        scene.deleteShape(static_cast<Shape *>(removed[i]));
    }

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
#pragma endregion

#pragma region shapes
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

    samples.push_back(vec3(0.82f, 0.24f, 0));
    samples.push_back(vec3(0.01f, 0.47f, 0));
    samples.push_back(vec3(-0.9f, 0.5f, 0));
    samples.push_back(vec3(-0.72f, 0.29f, 0));
    samples.push_back(vec3(-0.72f, -0.25f, 0));
    samples.push_back(vec3(-0.874f, -0.506f, 0));
    samples.push_back(vec3(0, -0.53f, 0));
    samples.push_back(vec3(0.82f, -0.29f, 0));

    derivs.push_back(vec2(0, 0.644f));
    derivs.push_back(vec2(-0.5426f, 0.0174f));
    derivs.push_back(vec2(-0.268f, 0.011f));
    derivs.push_back(vec2(-0.005f, -0.3315f));
    derivs.push_back(vec2(0.002f, -0.215f));
    derivs.push_back(vec2(-0.5048f, -0.05f));
    derivs.push_back(vec2(0.6037f, -0.002f));
    derivs.push_back(vec2(0, 0.644f));

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
    (*boat)->setAnchorPosition(BOAT_FIXED_X, getWaveHeight(BOAT_FIXED_X));
}

void createEnemies(Shape **e1, Shape **e2, Shape **e3)
{
    (*e1) = Shape::circle(&program, 4, vec4(1, 0, 0, 1), vec4(0, 1, 0, 1));
    (*e1)->setPosition((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT * 2 / 3);
    (*e1)->setScale(100);
    (*e1)->setRotation(radians(45.0f));
}

void createSea(Shape **sea)
{
    vector<vec3> vertices;
    vector<vec4> colors;

    vertices.push_back(vec3(0, -2.0f, 0));
    vertices.push_back(vec3(-1.0f, 0, 0));
    vertices.push_back(vec3(-1.0f, -1.0f, 0));
    vertices.push_back(vec3(1.0f + WAVE_WIDTH_NORM, -1.0f, 0));

    for (int i = 0; i < WAVES_POINTS; i++)
    {
        float x = (1.0f + WAVE_WIDTH_NORM) - (2.0f + WAVE_WIDTH_NORM) * i / (WAVES_POINTS - 1);
        vertices.push_back(vec3(x, getNormalizedWaveHeight(x), 0));
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0.3f, 0.75f, 0.8f));
    }

    (*sea) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*sea)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    // (*sea)->setScale(50, 50);
    (*sea)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
}
#pragma endregion

void createShapes()
{
    createBoat(&boat);
    createEnemies(&enemy1, &enemy2, &enemy3);
    createSea(&sea);

    scene.addShape(boat);
    scene.addShape(sea);
    scene.addShape(enemy1);
}

int main(int argc, char *argv[])
{
    w.init(argc, argv, title, 100, 100, drawCallback, updateCallback, 16, reshapeCallback);
    GLProgramInstance *inst = program.createProgram("default", vshader, fshader);
    program.useProgram(inst);
    cout << glGetError() << endl;

    glClearColor(0, 0.79f, 0.9f, 1.0f);

    createShapes();

    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(keyboardCallbackSp);
    glutSpecialUpFunc(keyboardCallbackUpSp);
    glutIgnoreKeyRepeat(1);

    w.loop();
}