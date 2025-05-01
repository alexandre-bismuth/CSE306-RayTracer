#ifndef node_h
#define node_h
#include "bounding_box.h"

class Node
{
public:
    Node() : leftChild(nullptr), rightChild(nullptr) {};

    int startTriangle, endTriangle;
    BoundingBox boundingBox;
    Node *leftChild, *rightChild;
};

#endif