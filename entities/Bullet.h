#include "../core/lib.h"

class Bullet
{
private:
    vec3 speedVec;
    float yGrav;

public:
    Bullet(float speed, float angle, float yGrav);

    vec3 getNewSpeed(float deltaT);
};
