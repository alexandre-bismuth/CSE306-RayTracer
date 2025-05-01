#include "headers/bounding_box.h"
#include <iostream>

bool BoundingBox::intersect(const Ray &ray, double &dist)
{ // Code based on page 44 of the textbook
    const double tx1 = (BMin[0] - ray.origin[0]) / ray.unitDirection[0];
    const double tx2 = (BMax[0] - ray.origin[0]) / ray.unitDirection[0];
    const double ty1 = (BMin[1] - ray.origin[1]) / ray.unitDirection[1];
    const double ty2 = (BMax[1] - ray.origin[1]) / ray.unitDirection[1];
    const double tz1 = (BMin[2] - ray.origin[2]) / ray.unitDirection[2];
    const double tz2 = (BMax[2] - ray.origin[2]) / ray.unitDirection[2];

    const double txmin = std::min(tx1, tx2);
    const double txmax = std::max(tx1, tx2);
    const double tymin = std::min(ty1, ty2);
    const double tymax = std::max(ty1, ty2);
    const double tzmin = std::min(tz1, tz2);
    const double tzmax = std::max(tz1, tz2);

    const double entry = std::max(std::max(tymin, tzmin), txmin);
    const double exit = std::min(std::min(tymax, tzmax), txmax);

    if (exit > entry && exit > 0)
    {
        dist = (entry > 0) ? entry : exit;
        return true;
    }
    return false;
};
