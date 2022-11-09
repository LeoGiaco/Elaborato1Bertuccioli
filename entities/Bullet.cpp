#include "Bullet.h"

Bullet::Bullet(float speed, float angle, float yGrav)
    : speedVec(vec3(speed * cos(angle), speed * sin(angle), 0)), yGrav(yGrav) {}

vec3 Bullet::getNewSpeed(float deltaT)
{
    speedVec.y -= deltaT * yGrav;
    return speedVec;
}