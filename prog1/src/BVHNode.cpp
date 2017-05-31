#include "BVHNode.h"


/*
std::vector<GeomObj*> sort_obj_on_axis(std::vector<GeomObj*> objList, int axis) {
    for (int i = 0; i < objList.size(); i++) {
        if (axis = 0) {
            
        }
        else if (axis = 1) {

        }
        else {

        }
    }
    return objList;
} */

std::vector<GeomObj*> left_half(std::vector<GeomObj*> objList) {
    size_t halfPos = objList.size()/2;
    std::vector<GeomObj*> newList = std::vector<GeomObj*>();
    if (halfPos > 0) {
        newList = std::vector<GeomObj*>(objList.begin(), objList.begin() + halfPos);
    }
    return newList;
}

std::vector<GeomObj*> right_half(std::vector<GeomObj*> objList) {
    size_t halfPos = objList.size()/2;
    std::vector<GeomObj*> newList = std::vector<GeomObj*>();
    if (halfPos > 0) {
        newList = std::vector<GeomObj*>(objList.begin() + halfPos, objList.end());
    }
    return newList;
}

BVHNode::BVHNode() {
    left = NULL;
    right = NULL;
    objList = std::vector<GeomObj*>();
}

void BVHNode::recursive_tree_build(std::vector<GeomObj*> oList, int axis) {
    if (oList.size() <= 1) {
        objList = oList;
    }
    else {
        left = new BVHNode();
        right = new BVHNode();
        left->recursive_tree_build(left_half(objList), (axis + 1) % 3);
        right->recursive_tree_build(right_half(objList), (axis + 1) % 3);
    }
}
