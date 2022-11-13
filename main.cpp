#include <iostream>
#include "./core/lib.h"
#include "./core/Window.h"
#include "./core/GLProgram.h"
#include "./core/Scene.h"
#include "./entities/Bullet.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define DELTA_T (16.0f / 1000)

#define WAVES_ON_SCREEN 3
#define WAVES_POINTS 100
#define WAVE_WIDTH_NORM (2.0f / WAVES_ON_SCREEN)
#define WAVE_WIDTH ((float)WINDOW_WIDTH / WAVES_ON_SCREEN) // WINDOW_WIDTH / 2.0f * WAVE_WIDTH_NORM
#define WAVE_MAX_HEIGHT_NORM (20.0f / WINDOW_HEIGHT)
#define WAVE_MAX_HEIGHT (WINDOW_HEIGHT * WAVE_MAX_HEIGHT_NORM)

#define WAVES_DIST 60

#define BOAT_FIXED_X (WINDOW_WIDTH / 5.0f)
#define MIN_ENEMY_X (WINDOW_WIDTH * 0.6f)

#define MOD(f1, f2) ((f1) - (f2)*floorf((f1) / (f2)))

#define BULLET_RELOAD 2.0f
#define BULLET_SPEED 600.0f
#define Y_FALL 600.0f

#define DEFAULT_SHADER_PROGRAM "default"
#define BOAT_SHADER_PROGRAM "boat"
#define SEA_SHADER_PROGRAM "sea"
#define ENEMY_SHADER_PROGRAM "enemy"
#define ENEMY_BOAT_SHADER_PROGRAM "enemyboat"

char *title = (char *)"Prova";

GLProgram program;
Scene scene;

mat4 projection = ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT);
Window w(&program, &scene, WINDOW_WIDTH, WINDOW_HEIGHT);

Shape *bg;

Shape *wave0, *wave1, *wave2, *wave3, *wave4;
float waveRelX0 = 0, waveRelX1 = 0, waveRelX2 = 0, waveRelX3 = 0, waveRelX4 = 0;

ComplexShape *boat;
float reload = 0;
int moving = 0, dir = 1;
float relBoatX = 0, boatMovementDeltaX = 0;
int cannonAngleMovement = 0;
list<tuple<Bullet *, Shape *>> projectiles;

#define INITIAL_ENEMY_X 1800.0f
#define ENEMY_X_DELTA 1000.0f

Shape *enemy1, *enemy2, *enemy3, **activeEnemy = &enemy1;
bool firstEnemy = true;
float enemyX = INITIAL_ENEMY_X;
float enemyHealth;

void createShaderPrograms()
{
    char *vshaderDef = (char *)"shaders/vertexShader.glsl";
    char *fshaderDef = (char *)"shaders/fragmentShader.glsl";
    char *vshaderBoat = (char *)"shaders/vertexShaderBoat.glsl";
    char *fshaderBoat = (char *)"shaders/fragmentShaderBoat.glsl";
    char *fshaderSea = (char *)"shaders/fragmentShaderWater.glsl";
    char *fshaderEnemy = (char *)"shaders/fragmentShaderEnemy.glsl";
    char *fshaderEnemyBoat = (char *)"shaders/fragmentShaderEnemyBoat.glsl";

    program.createProgram(DEFAULT_SHADER_PROGRAM, vshaderDef, fshaderDef)->setProjectionMatrix(projection);
    program.createProgram(BOAT_SHADER_PROGRAM, vshaderBoat, fshaderBoat)->setProjectionMatrix(projection);
    program.createProgram(SEA_SHADER_PROGRAM, vshaderDef, fshaderSea)->setProjectionMatrix(projection);
    program.createProgram(ENEMY_SHADER_PROGRAM, vshaderDef, fshaderEnemy)->setProjectionMatrix(projection);
    program.createProgram(ENEMY_BOAT_SHADER_PROGRAM, vshaderBoat, fshaderEnemyBoat)->setProjectionMatrix(projection);
}

void chooseNextEnemy()
{
    (*activeEnemy)->setEnabled(false);
    int i = 2;
    cout << i << endl;
    switch (i)
    {
    case 0:
        activeEnemy = &enemy1;
        break;
    case 1:
        activeEnemy = &enemy2;
        break;
    case 2:
        activeEnemy = &enemy3;
        break;
    }

    enemyHealth = 5;
    if (!firstEnemy)
    {
        enemyX += ENEMY_X_DELTA;
    }
    else
    {
        firstEnemy = false;
    }
    (*activeEnemy)->setEnabled(true);
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
        reload = BULLET_RELOAD;

        Shape *proj = Shape::circle(&program, 12, vec4(0, 0, 0, 1), vec4(0.3f, 0.3f, 0.3f, 0.3f));
        proj->setShaderProgram(DEFAULT_SHADER_PROGRAM);
        proj->setScale(7.5);
        proj->setAnchorPosition((*boat->getShape(4))->getWorldPosition());
        scene.addShape(proj);

        float angle = (*boat->getShape(4))->getCenterAngle();
        if (dir < 0)
            angle = radians(180.0f) - angle;

        Bullet *b = new Bullet(BULLET_SPEED, angle + (*boat->getShape(4))->getAnchorAngle(), Y_FALL);

        projectiles.push_back(make_tuple(b, proj));
    }
}

#pragma region callbacks
void drawCallback()
{
    w.drawScene();
}

void enemyReturnsNormal(int v)
{
    (*activeEnemy)->addUniformValue(VALUE_INT, "hit", new Value<int>(0));
}

void updateCallback(int v)
{
    if (reload > 0)
        reload -= DELTA_T;

    waveRelX0 -= 60 * DELTA_T;
    waveRelX1 -= 30 * DELTA_T;
    waveRelX2 -= 80 * DELTA_T;
    waveRelX3 -= 140 * DELTA_T;
    waveRelX4 -= 190 * DELTA_T;

    boatMovementDeltaX = moving * 200 * DELTA_T;
    if (enemyX - (relBoatX + boatMovementDeltaX) < MIN_ENEMY_X)
    {
        boatMovementDeltaX = enemyX - MIN_ENEMY_X - relBoatX;
    }
    relBoatX += boatMovementDeltaX;

    float angle = getWaveTangentAngle(BOAT_FIXED_X + relBoatX - waveRelX1);
    float wHeight = getWaveHeight(BOAT_FIXED_X + relBoatX - waveRelX1) - 10.0f;

    (*(boat->getShape(2)))->addUniformValue(VALUE_VEC2, "off", new Value<vec2>(vec2(BOAT_FIXED_X / WINDOW_WIDTH, wHeight / WINDOW_HEIGHT)));
    (*(boat->getShape(2)))->addUniformValue(VALUE_FLOAT, "angle", new Value<float>(angle));

    boat->setRotationAroundAnchor(angle);
    boat->setAnchorY(wHeight);

    if (cannonAngleMovement != 0)
    {
        Shape **cannon = boat->getShape(4);
        float cannonAngle = clamp((*cannon)->getCenterAngle() + cannonAngleMovement * radians(30 * DELTA_T), radians(-45.0f), radians(45.0f));
        (*cannon)->setRotation(cannonAngle);
    }

    wave0->setX(-MOD(relBoatX - waveRelX0, WAVE_WIDTH)); // The wave moves in the negative x.
    wave1->setX(-MOD(relBoatX - waveRelX1, WAVE_WIDTH)); // The wave moves in the negative x.
    wave2->setX(-MOD(relBoatX - waveRelX2, WAVE_WIDTH));
    wave3->setX(-MOD(relBoatX - waveRelX3, WAVE_WIDTH));
    wave4->setX(-MOD(relBoatX - waveRelX4, WAVE_WIDTH));

    (*activeEnemy)->setRotationAroundAnchor(getWaveTangentAngle(enemyX - waveRelX1));
    (*activeEnemy)->setAnchorX(enemyX - relBoatX);
    (*activeEnemy)->setAnchorY(getWaveHeight(enemyX - waveRelX1) + 30.0f);

    if (*activeEnemy == enemy3)
    {
        Shape **s = dynamic_cast<ComplexShape *>((*activeEnemy))->getShape(4);
        (*activeEnemy)->addUniformValue(VALUE_VEC2, "off", new Value<vec2>(vec2((*s)->getWorldPosition().x / WINDOW_WIDTH, (*s)->getWorldPosition().y / WINDOW_HEIGHT)));
        (*activeEnemy)->addUniformValue(VALUE_FLOAT, "angle", new Value<float>((*s)->getTotalAngle()));
    }

    vector<tuple<Bullet *, Shape *>> removed;

    auto it = projectiles.begin();
    for (size_t i = 0; i < projectiles.size(); i++)
    {
        auto tp = (*it);
        Bullet *b = get<0>(tp);
        Shape *p = get<1>(tp);

        p->move(b->getNewSpeed(DELTA_T) * DELTA_T);
        p->shiftX(-boatMovementDeltaX);

        if (p->getWorldPosition().y <= 0)
        {
            cout << "OUT " << endl;
            removed.push_back(tp);
            p->setEnabled(false);
        }

        if (p->isColliding((*activeEnemy)))
        {
            cout << "HIT " << endl;
            removed.push_back(tp);
            p->setEnabled(false);

            enemyHealth--;
            if (enemyHealth <= 0)
            {
                chooseNextEnemy();
            }
            else
            {
                (*activeEnemy)->addUniformValue(VALUE_INT, "hit", new Value<int>(1));
                glutTimerFunc(200, enemyReturnsNormal, 0);
            }
        }

        ++it;
    }

    for (size_t i = 0; i < removed.size(); i++)
    {
        projectiles.remove(removed[i]);
        scene.deleteShape(get<1>(removed[i]));
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
    case GLUT_KEY_UP:
        cannonAngleMovement = 1;
        break;
    case GLUT_KEY_DOWN:
        cannonAngleMovement = -1;
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
    case GLUT_KEY_UP:
        if (cannonAngleMovement > 0)
            cannonAngleMovement = 0;
        break;
    case GLUT_KEY_DOWN:
        if (cannonAngleMovement < 0)
            cannonAngleMovement = 0;
        break;
    default:
        break;
    }
}
#pragma endregion

#pragma region shapes
void createBackground(Shape **bg)
{
    vector<vec3> vertices;
    vector<vec4> colors;

    vertices.push_back(vec3(-1, -1, 0));
    vertices.push_back(vec3(1, -1, 0));
    vertices.push_back(vec3(1, 1, 0));
    vertices.push_back(vec3(1, 1, 0));
    vertices.push_back(vec3(-1, 1, 0));
    vertices.push_back(vec3(-1, -1, 0));

    for (size_t i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0.79f, 0.9f, 1.0f));
    }

    (*bg) = new Shape(&program, vertices, colors, GL_TRIANGLES);
    (*bg)->setShaderProgram(DEFAULT_SHADER_PROGRAM);
    (*bg)->setScale(WINDOW_WIDTH, WINDOW_HEIGHT);
    (*bg)->setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
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

    vec4 base_col(0.4f, 0.15f, 0.0f, 1.0f);
    vec4 stripe_col(0.35f, 0.0f, 0.0f, 1.0f);

    Shape *s1 = Shape::HermiteCurve(&program, 4, samples, derivs, false, vec4(), true, vec3(0, 0.2f, 0), vec4());
    s1->addUniformValue(VALUE_VEC2, "off", new Value<vec2>(vec2(BOAT_FIXED_X / WINDOW_WIDTH, 0.5f)));
    s1->addUniformValue(VALUE_INT, "stripes", new Value<int>(40));
    s1->addUniformValue(VALUE_FLOAT, "stripe_size", new Value<float>(0.2f));
    s1->addUniformValue(VALUE_FLOAT, "angle", new Value<float>(0.0f));
    s1->addUniformValue(VALUE_VEC4, "base_color", new Value<vec4>(base_col));
    s1->addUniformValue(VALUE_VEC4, "stripe_color", new Value<vec4>(stripe_col));
    s1->setShaderProgram(BOAT_SHADER_PROGRAM);
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
        colorsRect.push_back(vec4(0.25f, 0.0f, 0.0f, 1.0f));
    }

    Shape *s2 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s2->setShaderProgram(DEFAULT_SHADER_PROGRAM);
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

    Shape *s3 = Shape::HermiteCurve(&program, 4, samples, derivs, true, vec4(1, 1, 1, 1), true, vec3(0.6f, 0, 0), vec4(0.8, 0.8, 0.8, 1));
    s3->setShaderProgram(DEFAULT_SHADER_PROGRAM);
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
    s4->setShaderProgram(DEFAULT_SHADER_PROGRAM);
    s4->setScale(50);
    s4->setPosition(40, 40);
    //////////
    ////////// hole
    Shape *s5 = Shape::circle(&program, 12, vec4(0, 0, 0, 1), vec4(0, 0, 0, 1));
    s5->setShaderProgram(DEFAULT_SHADER_PROGRAM);
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
#pragma region enemy1
    vector<vec3> vertices;
    vector<vec4> colors;

    vertices.push_back(vec3(0, 0, 0));
    vertices.push_back(vec3(0, -0.3155f, 0));
    vertices.push_back(vec3(0.4896f, -0.2108f, 0));
    vertices.push_back(vec3(0.729f, 0.3835f, 0));
    vertices.push_back(vec3(0.4556f, 0.4379f, 0));
    vertices.push_back(vec3(0.4828f, 0.835f, 0));
    vertices.push_back(vec3(0.3672f, 0.918f, 0));
    vertices.push_back(vec3(-0.0789f, 0.7562f, 0));
    vertices.push_back(vec3(-0.3128, 0.9112f, 0));
    vertices.push_back(vec3(-0.4284f, 0.8935f, 0));
    vertices.push_back(vec3(-0.5549f, 0.4447f, 0));
    vertices.push_back(vec3(-0.748f, 0.1578f, 0));
    vertices.push_back(vec3(-0.4012f, -0.2938f, 0));
    vertices.push_back(vec3(0, -0.3155f, 0));

    colors.push_back(vec4(0, 0, 0, 1));
    for (int i = 0; i < vertices.size() - 1; i++)
    {
        colors.push_back(vec4(0.2f, 0.2f, 0.2f, 1.0f));
    }

    (*e1) = new Shape2D(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*e1)->addUniformValue(VALUE_INT, "hit", new Value<int>(0));
    (*e1)->setShaderProgram(ENEMY_SHADER_PROGRAM);
    (*e1)->setScale(100);
    (*e1)->setY(-20);
    (*e1)->setEnabled(false);
#pragma endregion

#pragma region enemy2
    vertices.clear();
    colors.clear();

    vertices.push_back(vec3(0, -0.05f, 0));
    vertices.push_back(vec3(-0.7808f, -0.0223f, 0));
    vertices.push_back(vec3(-0.9581f, -0.0447f, 0));
    vertices.push_back(vec3(-0.4723f, -0.2593f, 0));
    vertices.push_back(vec3(-0.0283f, -0.2637f, 0));
    vertices.push_back(vec3(0.365f, -0.1967f, 0));
    vertices.push_back(vec3(0.6809f, -0.0775f, 0));
    vertices.push_back(vec3(0.6809f, 0.0775f, 0));
    vertices.push_back(vec3(0.377f, 0.225f, 0));
    vertices.push_back(vec3(0.0447f, 0.2801f, 0));
    vertices.push_back(vec3(-0.5126f, 0.2831f, 0));
    vertices.push_back(vec3(-0.9715f, 0.1058f, 0));
    vertices.push_back(vec3(-0.8091f, 0.0372f, 0));
    vertices.push_back(vec3(-0.7808f, -0.0223f, 0));

    vec4 darkgray(0.3f, 0.3f, 0.3f, 1);
    vec4 lightgray(0.8f, 0.8f, 0.8f, 1);

    colors.push_back(darkgray);
    colors.push_back(darkgray);
    for (int i = 0; i < 5; i++)
    {
        colors.push_back(vec4(lightgray));
    }
    while (colors.size() < vertices.size())
    {
        colors.push_back(darkgray);
    }

    Shape *sharkBody = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    sharkBody->shiftY(-70);

    vector<vec3> finTailVertices;
    vector<vec4> finTailColors;

    finTailVertices.push_back(vec3(0.75f, 0, 0));
    finTailVertices.push_back(vec3(0.6809f, -0.0775f, 0));
    finTailVertices.push_back(vec3(0.9327f, -0.2607f, 0));
    finTailVertices.push_back(vec3(0.8329f, 0, 0));
    finTailVertices.push_back(vec3(0.9327f, 0.2771f, 0));
    finTailVertices.push_back(vec3(0.6809f, 0.0775f, 0));
    finTailVertices.push_back(vec3(0.6809f, -0.0775f, 0));

    for (size_t i = 0; i < finTailVertices.size() - 1; i++)
    {
        finTailColors.push_back(darkgray);
    }
    finTailColors.push_back(lightgray);
    finTailColors[1] = lightgray;

    Shape *sharkTailFin = new Shape(&program, finTailVertices, finTailColors, GL_TRIANGLE_FAN);
    sharkTailFin->setPosition(-0.02f, 0);
    sharkTailFin->shiftY(-70);

    vector<vec3> finBackVertices;
    vector<vec4> finBackColors;

    finBackVertices.push_back(vec3(0.0447f, 0.2801f, 0));
    finBackVertices.push_back(vec3(0.0536f, 0.5945f, 0));
    finBackVertices.push_back(vec3(-0.3084f, 0.2727f, 0));

    finBackColors.push_back(darkgray - vec4(0.1f, 0.1f, 0.1f, 0));
    finBackColors.push_back(darkgray);
    finBackColors.push_back(darkgray - vec4(0.1f, 0.1f, 0.1f, 0));

    Shape *sharkBackFin = new Shape(&program, finBackVertices, finBackColors, GL_TRIANGLES);
    sharkBackFin->setPosition(0, -0.02f);
    sharkBackFin->shiftY(-70);

    vertices.clear();
    colors.clear();

#define EYE_DELTA 0.03f

    vertices.push_back(vec3(-0.6958f, 0.1401, 0));
    vertices.push_back(vec3(-0.6958f - EYE_DELTA, 0.1401, 0));
    vertices.push_back(vec3(-0.6958f, 0.1401 - EYE_DELTA, 0));
    vertices.push_back(vec3(-0.6958f + EYE_DELTA, 0.1401, 0));
    vertices.push_back(vec3(-0.6958f, 0.1401 + EYE_DELTA, 0));
    vertices.push_back(vec3(-0.6958f - EYE_DELTA, 0.1401, 0));

    for (size_t i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0, 0, 1));
    }

    Shape *sharkEye = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    sharkEye->shiftY(-70);

    ComplexShape *shark = new ComplexShape(&program);
    shark->addShape(sharkBody);
    shark->addShape(sharkTailFin);
    shark->addShape(sharkBackFin);
    shark->addShape(sharkEye);
    shark->setScale(200);
    (*e2) = shark;
    (*e2)->addUniformValue(VALUE_INT, "hit", new Value<int>(0));
    (*e2)->setShaderProgram(ENEMY_SHADER_PROGRAM);
    (*e2)->setEnabled(false);
#pragma endregion

#pragma region enemy3
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

    vec4 base_col(0.1f, 0.1f, 0.2f, 1.0f);
    vec4 stripe_col(0.2f, 0.2f, 0.3f, 1.0f);

    Shape *s1 = Shape::HermiteCurve(&program, 4, samples, derivs, false, vec4(), true, vec3(0, 0.2f, 0), vec4());
    s1->addUniformValue(VALUE_VEC2, "off", new Value<vec2>(vec2(BOAT_FIXED_X / WINDOW_WIDTH, 0.5f)));
    s1->addUniformValue(VALUE_INT, "stripes", new Value<int>(10));
    s1->addUniformValue(VALUE_FLOAT, "stripe_size", new Value<float>(0.4f));
    s1->addUniformValue(VALUE_FLOAT, "angle", new Value<float>(0.0f));
    s1->addUniformValue(VALUE_VEC4, "base_color", new Value<vec4>(base_col));
    s1->addUniformValue(VALUE_VEC4, "stripe_color", new Value<vec4>(stripe_col));
    s1->setScale(100);
    //////////
    ////////// mainmast 1
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
        colorsRect.push_back(vec4(0.15f, 0.15f, 0.15f, 1.0f));
    }

    Shape *s2 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s2->setScale(80);
    s2->setPosition(-20, 110);
    //////////
    ////////// mainmast 2
    Shape *s2_2 = new Shape(&program, verticesRect, colorsRect, GL_TRIANGLE_FAN);
    s2_2->setScale(80, 80);
    s2_2->setPosition(50, 80);
    //////////
    ////////// sail 1
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

    Shape *s3 = Shape::HermiteCurve(&program, 4, samples, derivs, true, vec4(1, 1, 1, 1), true, vec3(0.6f, 0, 0), vec4(0.8, 0.8, 0.8, 1));
    s3->setScale(60);
    s3->setPosition(-20, 140);
    //////////
    ////////// sail 2
    Shape *s3_2 = Shape::HermiteCurve(&program, 4, samples, derivs, true, vec4(1, 1, 1, 1), true, vec3(0.6f, 0, 0), vec4(0.8, 0.8, 0.8, 1));
    s3_2->setScale(50);
    s3_2->setPosition(50, 110);
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

    ComplexShape *enemyBoat = new ComplexShape(&program);
    enemyBoat->addShape(s2);
    enemyBoat->addShape(s2_2);
    enemyBoat->addShape(s3);
    enemyBoat->addShape(s3_2);
    enemyBoat->addShape(s1);
    enemyBoat->addShape(s5);
    enemyBoat->addShape(s4);
    (*e3) = enemyBoat;
    (*e3)->addUniformValue(VALUE_INT, "hit", new Value<int>(0));
    (*e3)->setShaderProgram(ENEMY_SHADER_PROGRAM);
    s1->setShaderProgram(ENEMY_BOAT_SHADER_PROGRAM);
    (*e3)->shiftY(-30);
    (*e3)->setScaleRelativeToAnchor(-2, 2);
    (*e3)->setEnabled(false);
#pragma endregion
}

void createSea(Shape **wave0, Shape **wave1, Shape **wave2, Shape **wave3, Shape **wave4)
{
    vector<vec3> vertices;
    vector<vec4> colors;

    vertices.push_back(vec3(0, -2.0f, 0));
    vertices.push_back(vec3(-1.0f, 0, 0));
    vertices.push_back(vec3(-1.0f, -1.3f, 0));
    vertices.push_back(vec3(1.0f + WAVE_WIDTH_NORM, -1.3f, 0));

    for (int i = 0; i < WAVES_POINTS; i++)
    {
        float x = (1.0f + WAVE_WIDTH_NORM) - (2.0f + WAVE_WIDTH_NORM) * i / (WAVES_POINTS - 1);
        vertices.push_back(vec3(x, getNormalizedWaveHeight(x), 0));
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0.3f, 0.75f, 1.0f));
    }

    (*wave0) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*wave0)->setShaderProgram(SEA_SHADER_PROGRAM);
    (*wave0)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    (*wave0)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + WAVES_DIST);

    colors.clear();
    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0.1f, 0.55f, 1.0f));
    }

    (*wave1) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*wave1)->setShaderProgram(SEA_SHADER_PROGRAM);
    (*wave1)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    (*wave1)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    colors.clear();
    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0.1f, 0.55f, 1.0f));
    }

    (*wave2) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*wave2)->setShaderProgram(SEA_SHADER_PROGRAM);
    (*wave2)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    (*wave2)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - WAVES_DIST);

    colors.clear();
    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0, 0.3f, 1.0f));
    }

    (*wave3) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*wave3)->setShaderProgram(SEA_SHADER_PROGRAM);
    (*wave3)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    (*wave3)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 2 * WAVES_DIST);

    colors.clear();
    for (int i = 0; i < vertices.size(); i++)
    {
        colors.push_back(vec4(0, 0, 0, 1.0f));
    }

    (*wave4) = new Shape(&program, vertices, colors, GL_TRIANGLE_FAN);
    (*wave4)->setShaderProgram(SEA_SHADER_PROGRAM);
    (*wave4)->setScale(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    (*wave4)->setAnchorPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 3 * WAVES_DIST);
}
#pragma endregion

void createShapes()
{
    createBackground(&bg);
    createBoat(&boat);
    createEnemies(&enemy1, &enemy2, &enemy3);
    createSea(&wave0, &wave1, &wave2, &wave3, &wave4);

    scene.addShape(bg);
    scene.addShape(wave0);
    scene.addShape(boat);
    scene.addShape(enemy1);
    scene.addShape(enemy2);
    scene.addShape(enemy3);
    scene.addShape(wave4);
    scene.addShape(wave3);
    scene.addShape(wave2);
    scene.addShape(wave1);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    waveRelX0 = rand() % (int)WAVE_WIDTH;
    waveRelX1 = rand() % (int)WAVE_WIDTH;
    waveRelX2 = rand() % (int)WAVE_WIDTH;
    waveRelX3 = rand() % (int)WAVE_WIDTH;
    waveRelX4 = rand() % (int)WAVE_WIDTH;

    w.init(argc, argv, title, 100, 100, drawCallback, updateCallback, 16, reshapeCallback);

    createShaderPrograms();

    cout << glGetError() << endl;

    glClearColor(0, 0, 0, 0);

    createShapes();
    chooseNextEnemy();

    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(keyboardCallbackSp);
    glutSpecialUpFunc(keyboardCallbackUpSp);
    glutIgnoreKeyRepeat(1);

    w.loop();
}