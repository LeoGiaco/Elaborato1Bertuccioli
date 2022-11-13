#pragma once

#include "../core/lib.h"
#include "../core/GLProgram.h"

typedef int intVal;

#define VALUE_INT 0
#define VALUE_FLOAT 1
#define VALUE_VEC2 2
#define VALUE_VEC3 3
#define VALUE_VEC4 4
#define VALUE_MAT3 5
#define VALUE_MAT4 6

class ValueBase
{
public:
    virtual ~ValueBase() {}
};

template <typename T>
class Value : public ValueBase
{
    T value;

public:
    Value(T p)
    {
        value = p;
    }

    ~Value() {}

    T get()
    {
        return value;
    }
};

typedef class Shape2D
{
protected:
    GLuint VAO;
    GLuint VBO_vertices;
    GLuint VBO_colors;
    vector<vec3> vertices;
    vector<vec4> colors;
    map<string, tuple<intVal, ValueBase *>> uniformValues = map<string, tuple<intVal, ValueBase *>>();
    GLenum drawMode;
    mat4 model;

    GLProgram *program;
    string shapeShaders;

    vec3 scale;
    vec3 anchorScale;
    vec3 position;
    vec3 anchorPosition;
    float centerAngle;
    float anchorAngle;

    bool dynamicDraw;
    bool lockedRotation = false;
    bool updated;
    bool enabledNextFrame = true;
    bool enabled = true;

    void initShape();

    void sendUniformValues();

public:
    Shape2D(GLProgram *program, vector<vec3> vertices, vector<vec4> colors, GLenum drawMode, bool doDynamicDraw = false);

    // Adds a value to the list of uniform variables to be passed for the shaders.
    virtual void addUniformValue(int valueType, string uniformName, ValueBase *value);

    // Sets the chosen shader program to be used when drawing the shape.
    virtual void setShaderProgram(string shaderName);

    // Returns the angle of the shape around its center.
    virtual float getCenterAngle();
    // Returns the angle of the shape around its anchor.
    virtual float getAnchorAngle();
    // Returns the combined angle of the shape. If the rotation is locked, returns only the center angle.
    virtual float getTotalAngle();

    // Returns the value of the attribute position.
    virtual vec3 getPosition();

    // Returns the value of the attribute anchorPosition.
    virtual vec3 getAnchorPosition();

    // Returns the position of the shape, accounting for angles and positions.
    virtual vec3 getCombinedPosition();

    // Sets the scale of the shape.
    virtual void setScale(float scale);
    // Sets the scale of the shape.
    virtual void setScale(float xs, float ys, float zs = 1.0f);
    // Sets the scale of the shape.
    virtual void setScale(vec3 scale);

    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(float scale);
    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(float xs, float ys, float zs = 1.0f);
    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(vec3 scale);

    // Sets the position of the shape relative to the anchor.
    virtual void setPosition(float x, float y, float z = 0.0f);
    // Sets the position of the shape relative to the anchor.
    virtual void setPosition(vec3 pos);

    // Sets the x coordinate of the position of the shape.
    virtual void setX(float x);
    // Sets the y coordinate of the position of the shape.
    virtual void setY(float y);

    // Sets the x coordinate of the anchor position of the shape.
    virtual void setAnchorX(float x);
    // Sets the y coordinate of the anchor position of the shape.
    virtual void setAnchorY(float y);

    // Sets the anchor position of the shape.
    virtual void setAnchorPosition(float x, float y, float z = 0.0f);
    // Sets the anchor position of the shape.
    virtual void setAnchorPosition(vec3 anchorPos);

    // Sets the rotation of the shape around its center.
    virtual void setRotation(float radians);

    // Sets the rotation of the shape around its anchor.
    virtual void setRotationAroundAnchor(float radians);

    // Sets the lock on the rotation.
    // When set to true, rotation around the anchor no longer affects the normal rotation of the shape.
    virtual void setRotationLock(bool locked);

    // Changes the scale of the shape.
    virtual void changeScale(float scaleOff);
    // Changes the scale of the shape.
    virtual void changeScale(float xsOff, float ysOff, float zs_Off = 1.0f);
    // Changes the scale of the shape.
    virtual void changeScale(vec3 scaleOff);

    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(float scaleOff);
    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(float xsOff, float ysOff, float zs_Off = 1.0f);
    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(vec3 scaleOff);

    // Moves the shape's position on the x axis.
    virtual void shiftX(float xOff);
    // Moves the shape's position on the y axis.
    virtual void shiftY(float yOff);

    // Moves the shape's anchor position on the x axis.
    virtual void shiftAnchorX(float xOff);
    // Moves the shape's anchor position on the y axis.
    virtual void shiftAnchorY(float yOff);

    // Moves the shape.
    virtual void move(float xOff, float yOff, float zOff = 0.0f);
    // Moves the shape.
    virtual void move(vec3 offset);

    // Moves the shape's anchor.
    virtual void moveAnchor(float xOff, float yOff, float zOff = 0.0f);
    // Moves the shape's anchor.
    virtual void moveAnchor(vec3 offset);

    // Rotates the shape.
    virtual void rotate(float radians);

    // Rotates the shape around its anchor.
    virtual void rotateAroundAnchor(float radians);

    // Recalculates the model of the shape.
    virtual void calculateModelIfUpdated();

    // Creates the shape of a circle.
    static Shape2D *circle(GLProgram *program, int nvertices, vec4 centerColor, vec4 color);

    // Creates the shape of a Hermite curve.
    static Shape2D *HermiteCurve(GLProgram *program, int nPieceVertices, vector<vec3> samples, vector<vec2> derivatives, bool colorsEnabled = true,
                                 vec4 color = vec4(0, 0, 0, 1), bool fill = false, vec3 origin = vec3(0, 0, 0), vec4 centerColor = vec4(0, 0, 0, 0));

    // Determines whether the shape has to be drawn on screen.
    virtual void setEnabled(bool enabled);

    // Draws the shape.
    virtual void draw();

    // Returns a vector with the coordinates of the box containing the shape.
    // The vector contains (xMin, yMin) and (xMax, yMax).
    virtual vector<vec3> getBoxCollider();

    // Checks whether two shapes are intersecting with each other.
    bool isColliding(Shape2D *other);
} Shape;