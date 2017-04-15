#ifndef __Ray__
#define __Ray__

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Ray {

public:
    Ray();
    Ray(glm::vec3 p, glm::vec3 d);

    glm::vec3 get_pt() const {return pt;}
    glm::vec3 get_direction() const {return direction;}

    void set_pt(glm::vec3 p) {pt = glm::vec3(p.x, p.y, p.z);}
    void set_direction(glm::vec3 d) {direction = glm::vec3(d.x, d.y, d.z);}

    void print();

private:
    glm::vec3 pt, direction;

};

#endif