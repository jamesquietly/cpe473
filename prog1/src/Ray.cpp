#include "Ray.h"

Ray::Ray() {
    pt = glm::vec3(0, 0, 0);
    direction = glm::vec3(0, 0, 0);
}

Ray::Ray(glm::vec3 p, glm::vec3 d) {
    pt = glm::vec3(p.x, p.y, p.z);
    direction = glm::vec3(d.x, d.y, d.z);
}

void Ray::print() {
    std::cout << "Ray: {";
    std::cout << pt.x << " " << pt.y << " " << pt.z << "} -> {";
    std::cout << direction.x << " " << direction.y << " " << direction.z << "}";
}