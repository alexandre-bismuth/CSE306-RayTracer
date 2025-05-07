#ifndef SCENE_H
#define SCENE_H

#include "sphere.h"
#include "triangle_mesh.h"

class Scene
{
public:
    Scene(const Vector &lightPosition, unsigned long lightIntensity, bool useSpheres, bool useCat)
        : lightPosition(lightPosition), lightIntensity(lightIntensity), useSpheres(useSpheres), useCat(useCat) {};
    ~Scene();

    void initEnvironment();
    void initSpheres();
    void initCat();

    bool intersect(const Ray &ray, IntersectionInfo &rayIntersection);
    Vector getColor(const Ray &ray, int bounceNumber);

    const Vector lightPosition;
    const unsigned long lightIntensity;
    std::vector<Sphere> spheres;
    TriangleMesh *catMesh;
    const bool useSpheres, useCat;
};

#endif