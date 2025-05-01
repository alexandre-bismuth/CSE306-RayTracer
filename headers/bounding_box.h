#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "ray.h"

class BoundingBox
{
public:
    BoundingBox() {};
    bool intersect(const Ray &ray, double &dist);

    Vector BMin, BMax;
};

#endif