#ifndef __BVHNode__
#define __BVHNode__

#include "GeomObj.h"

//std::vector<GeomObj*> sort_obj_on_axis(std::vector<GeomObj*> objList, int axis);
std::vector<GeomObj*> left_half(std::vector<GeomObj*> objList);
std::vector<GeomObj*> right_half(std::vector<GeomObj*> objList);



class BVHNode {
public:
    BVHNode();
    BVHNode* left;
    BVHNode* right;
    std::vector<GeomObj*> objList;

    void recursive_tree_build(std::vector<GeomObj*>, int axis);
}; 

/*
struct BVHNode {
    BVHNode* left = NULL;
    BVHNode* right = NULL;
    std::vector<GeomObj*> objList = std::vector<GeomObj*>();

    BVHNode* recursive_tree_build(std::vector<GeomObj*> oList, int axis) {
        if (oList.size() <= 1) {
            this->objList = oList;
            return this;
        }

        this->left = new BVHNode();
        this->right = new BVHNode();
        this->left->recursive_tree_build(left_half(objList), (axis + 1) % 3);
        this->right->recursive_tree_build(right_half(objList), (axis + 1) % 3);
    }

    Box calc_bbox() {
        glm::vec3 minValues, maxValues;
        minValues = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxValues = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
        for (int i = 0; i < objList.size(); i++) {
            if () {

            }
        } 
    } 
}; 
 */
#endif