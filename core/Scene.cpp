#include "Scene.h"
#include <stdexcept>

Scene::Scene() {}

void Scene::addShape(Shape2D *shape, bool isForeground)
{
    if (isForeground)
        foregroundShapes.push_back(shape);
    else
        backgroundShapes.push_back(shape);
}

void Scene::removeShape(Shape2D *shape)
{
    backgroundShapes.remove(shape);
    foregroundShapes.remove(shape);
}

void Scene::deleteShape(Shape2D *shape)
{
    removeShape(shape);
    delete shape;
}

Shape2D *Scene::operator[](size_t index)
{
    if (index >= backgroundShapes.size())
    {
        throw runtime_error("Index outside of array.");
    }

    list<Shape2D *>::iterator it;

    if (index < backgroundShapes.size())
    {
        it = backgroundShapes.begin();
    }
    else
    {
        index -= backgroundShapes.size();

        it = foregroundShapes.begin();
    }

    for (size_t i = 0; i < index; i++)
    {
        ++it;
    }

    return *it;
}

size_t Scene::shapeNum()
{
    return backgroundShapes.size() + foregroundShapes.size();
}

void Scene::draw()
{
    list<Shape2D *>::iterator it = backgroundShapes.begin();
    for (size_t i = 0; i < backgroundShapes.size(); i++)
    {
        (*it)->draw();
        ++it;
    }

    it = foregroundShapes.begin();
    for (size_t i = 0; i < foregroundShapes.size(); i++)
    {
        (*it)->draw();
        ++it;
    }
}