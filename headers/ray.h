#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray(const Vector &origin, const Vector &unitDirection) : origin(origin), unitDirection(unitDirection) {};
    const Vector origin, unitDirection;
};

#endif