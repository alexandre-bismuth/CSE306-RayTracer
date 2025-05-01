#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "geometry.h"
#include "triangle_indices.h"
#include "bounding_box.h"
#include "node.h"

// Code based on the given Pastebin (https://pastebin.com/CAgp9r15)

class TriangleMesh : public Geometry
{
public:
    TriangleMesh(const Vector &center, const Vector &albedo, double scale = 1.,
                 bool isMirror = false, bool isTransparent = false,
                 bool invertNormal = false, bool isFresnel = false)
        : Geometry(center, albedo, isMirror, isTransparent, invertNormal, isFresnel), scale(scale) {};

    void readOBJ(const char *obj);
    bool intersect(const Ray &ray, IntersectionInfo &rayIntersection) override;
    BoundingBox computeBoundingBox(int &startTriangle, int &endTriangle);
    Vector computeBarycenter(TriangleIndices &index) const;
    void BVH(Node *node, int &startTriangle, int &endTriangle);
    void readPNG(const char *obj);
    Vector getTextureColor(double alpha, double beta, double gamma, const TriangleIndices &indices) const;

    const double scale;
    BoundingBox boundingBox;
    Node *rootNode;

    const unsigned char *texture;
    int textureWidth, textureHeight, *stbiComp;

    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices, normals, uvs, vertexcolors;
};

#endif