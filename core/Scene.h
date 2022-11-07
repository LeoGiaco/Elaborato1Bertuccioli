#pragma once

#include "lib.h"
#include "../shapes/Shape2D.h"
#include "../shapes/ComplexShape2D.h"

class Scene
{
private:
    list<Shape2D *> shapes;

public:
    Scene();

    void addShape(Shape2D *shape);

    // Removes the shape from the scene.
    void removeShape(Shape2D *shape);

    // Removes the shape from the scene and deletes it.
    void deleteShape(Shape2D *shape);

    Shape2D *operator[](size_t index);

    size_t shapeNum();

    void draw();
};
