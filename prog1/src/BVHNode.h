#ifndef __BVHNode__
#define __BVHNode__

#include "GeomObj.h"
#include "BBox.h"
bool sort_minx(GeomObj* a, GeomObj* b);
bool sort_miny(GeomObj* a, GeomObj* b);
bool sort_minz(GeomObj* a, GeomObj* b);
std::vector<GeomObj*> left_half(std::vector<GeomObj*> objList);
std::vector<GeomObj*> right_half(std::vector<GeomObj*> objList);
BBox calculateBBox(std::vector<GeomObj*> objList);


class BVHNode {
public:
    BVHNode();
    BVHNode* left;
    BVHNode* right;
    std::vector<GeomObj*> objList;
    BBox boundingBox;

    void recursive_tree_build(std::vector<GeomObj*>, int axis);
}; 


void tree_hits(BVHNode *node, Ray r, std::vector<GeomObj*> *objHitList);

#endif