#include "Scene.h"
#include <stdexcept>

Scene::Scene() {}

void Scene::addShape(Shape2D *shape)
{
    shapes.push_back(shape);
}

Shape2D *Scene::operator[](size_t index)
{
    if (index >= shapes.size())
    {
        throw runtime_error("Index outside of array.");
    }

    return shapes[index];
}

size_t Scene::shapeNum()
{
    return shapes.size();
}

void Scene::draw()
{
    for (size_t i = 0; i < shapeNum(); i++)
    {
        shapes[i]->draw();
    }
}