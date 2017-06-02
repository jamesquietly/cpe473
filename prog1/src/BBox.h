#ifndef __BBox__
#define __BBox__

#include "GeomObj.h"

class BBox {
public:
    BBox();
    BBox(glm::vec3 mins, glm::vec3 maxes);

    glm::vec3 get_min() const {return min;}
    glm::vec3 get_max() const {return max;}

    void set_min(glm::vec3 mi) {min = glm::vec3(mi);}
    void set_max(glm::vec3 ma) {max = glm::vec3(ma);}

    float intersect(Ray r);

private:
    glm::vec3 min, max;
};

#endif