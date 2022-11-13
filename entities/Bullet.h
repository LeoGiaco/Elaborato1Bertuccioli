#include "../core/lib.h"

class Bullet
{
private:
    vec3 speedVec;
    float yGrav;

public:
    Bullet(float speed, float angle, float yGrav);

    // Calculates the new speed after x milliseconds have passed.
    vec3 getNewSpeed(float deltaT);
};
