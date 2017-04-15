#ifndef __Light__
#define __Light__

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Light {
public:
    Light();
    Light(glm::vec3 l, glm::vec3 c);
    glm::vec3 get_loc() const {return loc;}
    glm::vec3 get_rgb() const {return rgb;}
    void set_loc(glm::vec3 v);
    void set_rgb(glm::vec3 v);

    void print();

private:
    glm::vec3 loc, rgb;
};

#endif