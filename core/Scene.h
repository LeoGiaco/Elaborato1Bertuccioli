#pragma once

#include "lib.h"
#include "../shapes/Shape2D.h"
#include "../shapes/ComplexShape2D.h"

class Scene
{
private:
    vector<Shape2D *> shapes;

public:
    Scene();
    void addShape(Shape2D *shape);
    Shape2D *operator[](size_t index);
    size_t shapeNum();
    void draw();
};
