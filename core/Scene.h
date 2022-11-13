#pragma once

#include "lib.h"
#include "../shapes/Shape2D.h"
#include "../shapes/ComplexShape2D.h"

class Scene
{
private:
    list<Shape2D *> backgroundShapes;
    list<Shape2D *> foregroundShapes;

public:
    Scene();

    // Adds a shape to the scene.
    void addShape(Shape2D *shape, bool isForeground = false);

    // Removes the shape from the scene.
    void removeShape(Shape2D *shape);

    // Removes the shape from the scene and deletes it.
    void deleteShape(Shape2D *shape);

    // Returns the shape with the given index.
    Shape2D *operator[](size_t index);

    // Returns the number of shapes in the scene.
    size_t shapeNum();

    // Draws all shapes in the scene.
    void draw();
};
