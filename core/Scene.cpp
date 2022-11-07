#include "Scene.h"
#include <stdexcept>

Scene::Scene() {}

void Scene::addShape(Shape2D *shape)
{
    shapes.push_back(shape);
}

void Scene::removeShape(Shape2D *shape)
{
    shapes.remove(shape);
}

void Scene::deleteShape(Shape2D *shape)
{
    removeShape(shape);
    delete shape;
}

Shape2D *Scene::operator[](size_t index)
{
    if (index >= shapes.size())
    {
        throw runtime_error("Index outside of array.");
    }

    list<Shape2D *>::iterator it = shapes.begin();
    for (size_t i = 0; i < index; i++)
    {
        ++it;
    }

    return *it;
}

size_t Scene::shapeNum()
{
    return shapes.size();
}

void Scene::draw()
{
    list<Shape2D *>::iterator it = shapes.begin();
    for (size_t i = 0; i < shapeNum(); i++)
    {
        (*it)->draw();
        ++it;
    }
}