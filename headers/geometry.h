#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"
#include "utils.h"

class Geometry
{
public:
    Geometry(const Vector &center, const Vector &albedo,
             bool isMirror = false, bool isTransparent = false,
             bool invertNormal = false, bool isFresnel = false)
        : center(center), albedo(albedo), isMirror(isMirror), isTransparent(isTransparent), invertNormal(invertNormal), isFresnel(isFresnel) {};

    virtual bool intersect(const Ray &ray, IntersectionInfo &rayIntersection) = 0;

    const Vector center, albedo;
    const bool isMirror, isTransparent, invertNormal, isFresnel;
};

#endif