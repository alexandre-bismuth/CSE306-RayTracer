#include "headers/sphere.h"

bool Sphere::intersect(const Ray &ray, IntersectionInfo &rayIntersection)
{
    const double delta = sqr(dot(ray.unitDirection, ray.origin - this->center)) - ((ray.origin - this->center).norm2() - sqr(this->radius));
    if (delta < 0)
        return false;

    const double proj = dot(ray.unitDirection, this->center - ray.origin);
    const double entryDist = proj - sqrt(delta);
    const double exitDist = proj + sqrt(delta);

    if (exitDist < 0)
        return false;

    rayIntersection.dist = (entryDist > 0) ? entryDist : exitDist;
    rayIntersection.inter = ray.origin + ray.unitDirection * rayIntersection.dist;
    rayIntersection.interNormal = (invertNormal) ? center - rayIntersection.inter : rayIntersection.inter - center;
    rayIntersection.interNormal.normalize();

    return true;
}