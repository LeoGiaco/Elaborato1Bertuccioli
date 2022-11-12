#include "Shape2D.h"

typedef class ComplexShape2D : public Shape2D
{
private:
    vector<Shape2D *> shapes;

public:
    ComplexShape2D(GLProgram *program);

    void addShape(Shape2D *shape);

    virtual void addUniformValue(int valueType, string uniformName, ValueBase *value);

    virtual void setShaderProgram(string shaderName);

    Shape **getShape(int shapeNum);

    // Sets the scale of all sub-shapes.
    // Should not be used in most cases in favor of setScaleAroundAnchor.
    void setScale(float scale) override;
    // Sets the scale of all sub-shapes.
    // Should not be used in most cases in favor of setScaleAroundAnchor.
    void setScale(float xs, float ys, float zs = 1.0f) override;
    // Sets the scale of all sub-shapes.
    // Should not be used in most cases in favor of setScaleAroundAnchor.
    void setScale(vec3 scale) override;

    // Sets the scale of all sub-shapes using the anchor as center.
    void setScaleRelativeToAnchor(float scale);
    // Sets the scale of all sub-shapes using the anchor as center.
    void setScaleRelativeToAnchor(float xs, float ys, float zs = 1.0f);
    // Sets the scale of all sub-shapes using the anchor as center.
    void setScaleRelativeToAnchor(vec3 scale);

    // Sets the position of all sub-shapes relative to their anchor.
    // Should not be used in most cases in favor of setAnchorPosition.
    void setPosition(float x, float y, float z = 0.0f) override;
    // Sets the position of all sub-shapes relative to their anchor.
    // Should not be used in most cases in favor of setAnchorPosition.
    void setPosition(vec3 pos) override;

    void setX(float x);
    void setY(float y);

    void setAnchorX(float x);
    void setAnchorY(float y);

    // Sets the anchor position of all sub-shapes.
    void setAnchorPosition(float x, float y, float z = 0.0f) override;
    // Sets the anchor position of all sub-shapes.
    void setAnchorPosition(vec3 anchorPos) override;

    // Sets the rotation of all sub-shapes around their center.
    // Should not be used in most cases in favor of setRotationAroundAnchor.
    void setRotation(float radians) override;

    // Sets the rotation of all sub-shapes around their anchor.
    void setRotationAroundAnchor(float radians) override;

    // Sets the lock on the rotation for all sub-shapes.
    // When set to true, rotation around the anchor no longer affects the normal rotation of all sub-shapes.
    // Should not be used in most cases.
    void setRotationLock(bool locked) override;

    // Changes the scale of all sub-shapes.
    // Should not be used in most cases in favor of changeScaleRelativeToAnchor.
    void changeScale(float scaleOff) override;
    // Changes the scale of all sub-shapes.
    // Should not be used in most cases in favor of changeScaleRelativeToAnchor.
    void changeScale(float xsOff, float ysOff, float zs_Off = 1.0f) override;
    // Changes the scale of all sub-shapes.
    // Should not be used in most cases in favor of changeScaleRelativeToAnchor.
    void changeScale(vec3 scaleOff) override;

    // Changes the scale of all sub-shapes using their anchor as center.
    void changeScaleRelativeToAnchor(float scaleOff) override;
    // Changes the scale of all sub-shapes using their anchor as center.
    void changeScaleRelativeToAnchor(float xsOff, float ysOff, float zs_Off = 1.0f) override;
    // Changes the scale of all sub-shapes using their anchor as center.
    void changeScaleRelativeToAnchor(vec3 scaleOff) override;

    // Moves all sub-shapes' position on the x axis.
    // Should not be used in most cases in favor of shiftAnchorX.
    void shiftX(float xOff) override;
    // Moves all sub-shapes' position on the y axis.
    // Should not be used in most cases in favor of shiftAnchorY.
    void shiftY(float yOff) override;

    // Moves all sub-shapes' anchor position on the x axis.
    void shiftAnchorX(float xOff) override;
    // Moves all sub-shapes' anchor position on the y axis.
    void shiftAnchorY(float yOff) override;

    // Moves all sub-shapes.
    // Should not be used in most cases in favor of moveAnchor.
    void move(float xOff, float yOff, float zOff = 0.0f) override;
    // Moves all sub-shapes.
    // Should not be used in most cases in favor of moveAnchor.
    void move(vec3 offset) override;

    // Moves all sub-shapes' anchor.
    void moveAnchor(float xOff, float yOff, float zOff = 0.0f) override;
    // Moves all sub-shapes' anchor.
    void moveAnchor(vec3 offset) override;

    // Rotates all sub-shapes.
    // Should not be used in most cases in favor of rotateAroundAnchor.
    void rotate(float radians) override;

    // Rotates all sub-shapes around their anchor.
    void rotateAroundAnchor(float radians) override;

    // Recalculates the model of all sub-shapes.
    void calculateModelIfUpdated() override;

    void setEnabled(bool enabled);

    vector<vec3> getBoxCollider();

    // Draws all sub-shapes.
    virtual void draw() override;
} ComplexShape;
