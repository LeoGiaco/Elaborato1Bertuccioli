#include "ComplexShape2D.h"

ComplexShape2D::ComplexShape2D(GLProgram *program)
    : Shape2D(program, vector<vec3>(), vector<vec4>(), 0, false), shapes(vector<Shape2D *>())
{
}

void ComplexShape::addShape(Shape2D *shape)
{
    shapes.push_back(shape);
}

Shape **ComplexShape::getShape(int shapeNum)
{
    return &(shapes[shapeNum]);
}

void ComplexShape2D::setScale(float scale)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScale(scale);
    }
}

void ComplexShape2D::setScale(float xs, float ys, float zs)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScale(xs, ys, zs);
    }
}

void ComplexShape2D::setScale(vec3 scale)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScale(scale);
    }
}

void ComplexShape2D::setScaleRelativeToAnchor(float scale)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScaleRelativeToAnchor(scale);
    }
}

void ComplexShape2D::setScaleRelativeToAnchor(float xs, float ys, float zs)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScaleRelativeToAnchor(xs, ys, zs);
    }
}

void ComplexShape2D::setScaleRelativeToAnchor(vec3 scale)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setScaleRelativeToAnchor(scale);
    }
}

void ComplexShape2D::setPosition(float x, float y, float z)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setPosition(x, y, z);
    }
}

void ComplexShape2D::setPosition(vec3 pos)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setPosition(pos);
    }
}

void ComplexShape2D::setX(float x)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setX(x);
    }
}

void ComplexShape2D::setY(float y)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setY(y);
    }
}

void ComplexShape2D::setAnchorX(float x)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setAnchorX(x);
    }
}

void ComplexShape2D::setAnchorY(float y)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setAnchorY(y);
    }
}

void ComplexShape2D::setAnchorPosition(float x, float y, float z)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setAnchorPosition(x, y, z);
    }
}

void ComplexShape2D::setAnchorPosition(vec3 anchorPos)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setAnchorPosition(anchorPos);
    }
}

void ComplexShape2D::setRotation(float radians)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setRotation(radians);
    }
}

void ComplexShape2D::setRotationAroundAnchor(float radians)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setRotationAroundAnchor(radians);
    }
}

void ComplexShape2D::setRotationLock(bool locked)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->setRotationLock(locked);
    }
}

void ComplexShape2D::changeScale(float scaleOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScale(scaleOff);
    }
}

void ComplexShape2D::changeScale(float xsOff, float ysOff, float zsOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScale(xsOff, ysOff, zsOff);
    }
}

void ComplexShape2D::changeScale(vec3 scaleOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScale(scaleOff);
    }
}

void ComplexShape2D::changeScaleRelativeToAnchor(float scaleOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScaleRelativeToAnchor(scaleOff);
    }
}

void ComplexShape2D::changeScaleRelativeToAnchor(float xsOff, float ysOff, float zsOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScaleRelativeToAnchor(xsOff, ysOff, zsOff);
    }
}

void ComplexShape2D::changeScaleRelativeToAnchor(vec3 scaleOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->changeScaleRelativeToAnchor(scaleOff);
    }
}

void ComplexShape2D::shiftX(float xOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->shiftX(xOff);
    }
}

void ComplexShape2D::shiftY(float yOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->shiftY(yOff);
    }
}

void ComplexShape2D::shiftAnchorX(float xOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->shiftAnchorX(xOff);
    }
}

void ComplexShape2D::shiftAnchorY(float yOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->shiftAnchorY(yOff);
    }
}

void ComplexShape2D::move(float xOff, float yOff, float zOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->move(xOff, yOff, zOff);
    }
}

void ComplexShape2D::move(vec3 offset)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->move(offset);
    }
}

void ComplexShape2D::moveAnchor(float xOff, float yOff, float zOff)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->moveAnchor(xOff, yOff, zOff);
    }
}

void ComplexShape2D::moveAnchor(vec3 offset)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->moveAnchor(offset);
    }
}

void ComplexShape2D::rotate(float radians)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->rotate(radians);
    }
}

void ComplexShape2D::rotateAroundAnchor(float radians)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->rotateAroundAnchor(radians);
    }
}

void ComplexShape2D::calculateModelIfUpdated()
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        shapes[i]->calculateModelIfUpdated();
    }
}

void ComplexShape2D::draw()
{
    if (enabled)
    {
        for (size_t i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }
    }
}