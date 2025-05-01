#ifndef SPHERE_H
#define SPHERE_H

#include "geometry.h"

class Sphere : public Geometry
{
public:
    Sphere(const Vector &center, double radius, const Vector &albedo,
           bool isMirror = false, bool isTransparent = false,
           bool invertNormal = false, bool isFresnel = false)
        : Geometry(center, albedo, isMirror, isTransparent, invertNormal, isFresnel), radius(radius) {};

    bool intersect(const Ray &ray, IntersectionInfo &rayIntersection) override;

    const double radius;
};

#endif