#include "Light.h"

Light::Light() {
    loc = glm::vec3(0, 0, 0);
    rgb = glm::vec3(0, 0, 0);
}

Light::Light(glm::vec3 l, glm::vec3 c) {
    loc = glm::vec3(l.x, l.y, l.z);
    rgb = glm::vec3(c.x, c.y, c.z);
}

void Light::set_loc(glm::vec3 v) {
    loc = glm::vec3(v.x, v.y, v.z);
}

void Light::set_rgb(glm::vec3 v) {
    rgb = glm::vec3(v.x, v.y, v.z);
}

void Light::print() {
    std::cout << "- Location: {";
    std::cout << loc.x << " " << loc.y << " " << loc.z << "}\n";
    std::cout << "- Color: {";
    std::cout << rgb.x << " " << rgb.y << " " << rgb.z << "}\n";
}

