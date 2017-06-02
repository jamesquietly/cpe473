#include "BBox.h"

BBox::BBox() {
    min = glm::vec3(0, 0, 0);
    max = glm::vec3(0, 0, 0);
}

BBox::BBox(glm::vec3 mins, glm::vec3 maxes) {
    min = glm::vec3(mins);
    maxes = glm::vec3(maxes);
}

float BBox::intersect(Ray r) {
    glm::vec3 rayPt, rayDir;
    rayPt = r.get_pt();
    rayDir = r.get_direction();
    float returnVal = -1;

    if (rayDir.x == 0) {
        if (rayPt.x >= min.x || rayPt.x <= max.x) {
            return -1;
        }
    }
    if (rayDir.y == 0) {
        if (rayPt.y >= min.y || rayPt.y <= max.y) {
            return -1;
        }
    }
    if (rayDir.z == 0) {
        if (rayPt.z >= min.z || rayPt.z <= max.z) {
            return -1;
        }
    }
    else {
        float tgmin = -std::numeric_limits<float>::infinity();
        float tgmax = std::numeric_limits<float>::infinity();

        //for x
        float t1 = (min.x - rayPt.x)/rayDir.x;
        float t2 = (max.x - rayPt.x)/rayDir.x;

        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }

        //for y
        t1 = (min.y - rayPt.y)/rayDir.y;
        t2 = (max.y - rayPt.y)/rayDir.y;
        
        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }

        //for z
        t1 = (min.z - rayPt.z)/rayDir.z;
        t2 = (max.z - rayPt.z)/rayDir.z;
        
        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }


        if (tgmin > tgmax) {
            return -1;
        }
        //box is behind
        if (tgmax < 0) {
            return -1;
        }

        //if we got here, there's an intersection
        if (tgmin > 0) {
            returnVal = tgmin;
        }
        else {
            returnVal = tgmax; //could be inside, so tgmax is the intersection we will hit
        }
    }
    return returnVal;
}