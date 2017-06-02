#include "BVHNode.h"

bool sort_minx(GeomObj* a, GeomObj* b) {
    return a->get_min().x < b->get_min().x;
}

bool sort_miny(GeomObj* a, GeomObj* b) {
    return a->get_min().y < b->get_min().y;
}

bool sort_minz(GeomObj* a, GeomObj* b) {
    return a->get_min().z < b->get_min().z;
}


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

BBox calculateBBox(std::vector<GeomObj*> objList) {
    glm::vec3 min, max, objMin, objMax;
    min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    for (int i = 0; i < objList.size(); i++) {
        objMin = objList[i]->get_min();
        objMax = objList[i]->get_max();
        if (objMin.x < min.x) {
            min.x = objMin.x;
        }
        if (objMin.y < min.y) {
            min.y = objMin.y;
        }
        if (objMin.z < min.z) {
            min.z = objMin.z;
        }

        if (objMax.x > max.x) {
            max.x = objMax.x;
        }
        if (objMax.y > max.y) {
            max.y = objMax.y;
        }
        if (objMax.z > max.z) {
            max.z = objMax.z;
        }
    }
    return BBox(min, max);
}

void tree_hits(BVHNode *node, Ray r, std::vector<GeomObj*> *objHitList) {
    float leafNodeT;
    //check if ray hits root
    if (node->boundingBox.intersect(r) != -1) {
        if (node->left != NULL) {
            //check if ray hits left side
            if (node->left->boundingBox.intersect(r) != -1) {
                tree_hits(node->left, r, objHitList);
            }
        }
        if (node->right != NULL) {
            //check if ray hits right side
            if (node->right->boundingBox.intersect(r) != -1) {
                tree_hits(node->right, r, objHitList);
            }
        }
        //check if ray hits leaf node
        if (node->left == NULL && node->right == NULL) {
            leafNodeT = node->objList[0]->intersect(r);
            if (leafNodeT != -1) {
                objHitList->push_back(node->objList[0]);
            }
        }
    }
}

BVHNode::BVHNode() {
    left = NULL;
    right = NULL;
    objList = std::vector<GeomObj*>();
    boundingBox = BBox();
}

void BVHNode::recursive_tree_build(std::vector<GeomObj*> oList, int axis) {
    if (oList.size() <= 1) {
        objList = oList;
        boundingBox = calculateBBox(oList);
    }
    else {
        if (axis == 0) {
            std::sort(oList.begin(), oList.end(), sort_minx);
        }
        else if (axis == 1) {
            std::sort(oList.begin(), oList.end(), sort_miny);
        }
        else {
            std::sort(oList.begin(), oList.end(), sort_minz);
        }

        left = new BVHNode();
        right = new BVHNode();
        left->recursive_tree_build(left_half(oList), (axis + 1) % 3);
        right->recursive_tree_build(right_half(oList), (axis + 1) % 3);
        boundingBox = calculateBBox(oList);
    }
}
