#pragma once

#include "../core/lib.h"
#include "../core/GLProgram.h"

typedef class Shape2D
{
protected:
    GLuint VAO;
    GLuint VBO_vertices;
    GLuint VBO_colors;
    vector<vec3> vertices;
    vector<vec4> colors;
    GLenum drawMode;

    GLProgram *program;

    mat4 model;

    vec3 scale;
    vec3 anchorScale;
    vec3 position;
    vec3 anchorPosition;
    float centerAngle;
    float anchorAngle;

    bool dynamicDraw;
    bool lockedRotation = false;
    bool updated;
    bool enabled = true;

    void initShape();

public:
    Shape2D(GLProgram *program, vector<vec3> vertices, vector<vec4> colors, GLenum drawMode, bool doDynamicDraw = false);

    // Returns the angle of the shape around its center.
    float getCenterAngle();
    // Returns the angle of the shape around its anchor.
    float getAnchorAngle();
    // Returns the combined angle of the shape. If the rotation is locked, returns only the center angle.
    float getTotalAngle();

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
    static Shape2D *HermiteCurve(GLProgram *program, int nPieceVertices, vector<vec3> samples, vector<vec2> derivatives,
                                 vec4 color, bool fill = false, vec3 origin = vec3(0, 0, 0), vec4 centerColor = vec4(0, 0, 0, 0));

    // Determines whether the shape has to be drawn on screen.
    virtual void setEnabled(bool enabled);

    // Draws the shape.
    virtual void draw();
} Shape;