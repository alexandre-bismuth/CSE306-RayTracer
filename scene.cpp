#include "headers/scene.h"
#include "headers/utils.h"

Scene::~Scene()
{
    if (catMesh)
    {
        delete catMesh->rootNode;
        delete catMesh;
    }
}

bool Scene::intersect(const Ray &ray, IntersectionInfo &rayIntersection)
{
    bool intersected = false;
    rayIntersection.dist = std::numeric_limits<double>::max();

    if (useCat)
    {
        IntersectionInfo currentIntersection;
        if (catMesh->intersect(ray, currentIntersection))
        {
            intersected = true;
            if (currentIntersection.dist < rayIntersection.dist)
            {
                rayIntersection = currentIntersection;
                rayIntersection.objectId = -1; // We assume that negative ids designates the cat
            }
        }
    }

    for (size_t i = 0; i < spheres.size(); i++)
    {
        IntersectionInfo currentIntersection;
        if (spheres[i].intersect(ray, currentIntersection))
        {
            intersected = true;
            if (currentIntersection.dist < rayIntersection.dist)
            {
                rayIntersection = currentIntersection;
                rayIntersection.objectId = int(i);
            }
        }
    }
    return intersected;
}

Vector Scene::getColor(const Ray &ray, int bounceNumber)
{
    if (bounceNumber <= 0)
        return Vector();
    const double epsilon = 1e-5;
    const unsigned char threadId = omp_get_thread_num();

    IntersectionInfo rayIntersection;
    if (!intersect(ray, rayIntersection))
        return Vector();

    // Check if we are dealing with a sphere or a cat
    const bool isSphere = rayIntersection.objectId >= 0;
    Vector objectAlbedo;

    if (isSphere)
    {
        objectAlbedo = spheres[rayIntersection.objectId].albedo;
        if (spheres[rayIntersection.objectId].isMirror)
        { // Based on formulas from page 19 of the textbook
            const Vector reflectedDir = ray.unitDirection - 2 * rayIntersection.interNormal * dot(ray.unitDirection, rayIntersection.interNormal);
            const Ray reflectedRay(rayIntersection.inter + epsilon * rayIntersection.interNormal, reflectedDir);
            return getColor(reflectedRay, bounceNumber - 1);
        }

        if (spheres[rayIntersection.objectId].isTransparent)
        { // Based on formulas from page 20 of the textbook
            double refractionAir = 1.;
            double refractionGlass = 1.5;
            if (dot(ray.unitDirection, rayIntersection.interNormal) > 0)
            {
                std::swap(refractionAir, refractionGlass);
                rayIntersection.interNormal = -rayIntersection.interNormal;
            }
            const Vector tangentialComponent = refractionAir / refractionGlass * (ray.unitDirection - dot(ray.unitDirection, rayIntersection.interNormal) * rayIntersection.interNormal);
            const double delta = 1. - sqr(refractionAir / refractionGlass) * (1. - sqr(dot(ray.unitDirection, rayIntersection.interNormal)));
            if (delta < 0)
            {
                Vector reflectedDir = ray.unitDirection - 2. * rayIntersection.interNormal * dot(ray.unitDirection, rayIntersection.interNormal);
                Ray reflectedRay(rayIntersection.inter + epsilon * rayIntersection.interNormal, reflectedDir);
                return getColor(reflectedRay, bounceNumber - 1);
            }

            const Vector normalComponent = -sqrt(delta) * rayIntersection.interNormal;
            const Vector refractedDir = tangentialComponent + normalComponent;

            if (spheres[rayIntersection.objectId].isFresnel)
            { // Based on page 21 from the textbook
                const double k0 = sqr((refractionAir - refractionGlass) / (refractionAir + refractionGlass));
                const double R = k0 + (1. - k0) * pow(1. - abs(dot(ray.unitDirection, rayIntersection.interNormal)), 5.);
                const double random = uniform(engine[threadId]);

                const Vector reflectedDir = ray.unitDirection - 2. * rayIntersection.interNormal * dot(ray.unitDirection, rayIntersection.interNormal);
                if (random < R)
                {
                    Ray reflectedRay(rayIntersection.inter + epsilon * rayIntersection.interNormal, reflectedDir);
                    return getColor(reflectedRay, bounceNumber - 1);
                }
                else
                {
                    Ray refractedRay(rayIntersection.inter - epsilon * rayIntersection.interNormal, refractedDir);
                    return getColor(refractedRay, bounceNumber - 1);
                }
            }
            else
            {
                Ray refractedRay(rayIntersection.inter - epsilon * rayIntersection.interNormal, refractedDir);
                return getColor(refractedRay, bounceNumber - 1);
            }
        }
    }
    else
    {
        const TriangleIndices &indices = catMesh->indices[rayIntersection.triangleId];
        objectAlbedo = catMesh->getTextureColor(rayIntersection.alpha, rayIntersection.beta, rayIntersection.gamma, indices);
    }

    Vector lightDir = lightPosition - rayIntersection.inter;
    const double dirNorm2 = lightDir.norm2();
    lightDir.normalize();
    Vector color = lightIntensity / (4 * M_PI * dirNorm2) * objectAlbedo / M_PI * std::max(dot(lightDir, rayIntersection.interNormal), 0.);

    // Shadows implementation - page 17 of the textbook
    const Ray shadowRay(rayIntersection.inter + epsilon * rayIntersection.interNormal, lightDir);
    IntersectionInfo shadowIntersection;
    if (intersect(shadowRay, shadowIntersection) && sqr(shadowIntersection.dist) < dirNorm2)
        color = Vector();

    // Indirect lighting implementation - page 22 of the textbook
    const Ray indirectRay(rayIntersection.inter + epsilon * rayIntersection.interNormal, randomCos(rayIntersection.interNormal));
    color += objectAlbedo * getColor(indirectRay, bounceNumber - 1);
    return color;
}

void Scene::initEnvironment()
{
    // Based on the scenario from the textbook
    spheres.push_back(Sphere(Vector(1000, 0, 0), 940, Vector(0.6, 0.5, 0.1)));  // Left-hand wall
    spheres.push_back(Sphere(Vector(-1000, 0, 0), 940, Vector(0.9, 0.2, 0.9))); // Right-hand wall
    spheres.push_back(Sphere(Vector(0, 1000, 0), 940, Vector(0.2, 0.5, 0.9)));  // Ceiling
    spheres.push_back(Sphere(Vector(0, -1000, 0), 990, Vector(0.3, 0.4, 0.7))); // Floor
    spheres.push_back(Sphere(Vector(0, 0, -1000), 940, Vector(0.4, 0.8, 0.7))); // Back wall
    spheres.push_back(Sphere(Vector(0, 0, 1000), 940, Vector(0.9, 0.4, 0.3)));  // Wall behind camera

    if (useSpheres)
        initSpheres();
    if (useCat)
        initCat();
}

void Scene::initSpheres()
{
    spheres.push_back(Sphere(Vector(0, 0, 0), 10, Vector(1, 1, 1)));                            // Diffuse Sphere
    spheres.push_back(Sphere(Vector(-20, 0, 0), 10, Vector(1, 1, 1), true));                    // Mirror Sphere
    spheres.push_back(Sphere(Vector(20, 0, 0), 10, Vector(1, 1, 1), false, true, false, true)); // Transparent Sphere (base)
    spheres.push_back(Sphere(Vector(20, 0, 0), 9.9, Vector(1, 1, 1), false, true, true, true)); // Transparent Sphere (inverted)
}

void Scene::initCat()
{
    catMesh = new TriangleMesh(Vector(0, -10.5, 30), Vector(1, 1, 1), 0.33);
    catMesh->readOBJ("resources/cat/cat.obj");
    catMesh->readPNG("resources/cat/cat_diff.png");
    catMesh->rootNode = new Node();
    int startTriangle = 0;
    int endTriangle = int(catMesh->indices.size());
    catMesh->BVH(catMesh->rootNode, startTriangle, endTriangle);
}