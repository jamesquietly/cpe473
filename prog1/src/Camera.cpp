#include "Camera.h"

Camera::Camera() {
    loc = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 0, 0);
    right = glm::vec3(0, 0, 0);
    lookAt = glm::vec3(0, 0, 0);

}

Camera::Camera(glm::vec3 lo, glm::vec3 u, glm::vec3 r, glm::vec3 la) {
    loc = glm::vec3(lo.x, lo.y, lo.z);
    up = glm::vec3(u.x, u.y, u.z);
    right = glm::vec3(r.x, r.y, r.z);
    lookAt = glm::vec3(la.x, la.y, la.z);

}

void Camera::set_loc(glm::vec3 v) {
    loc = glm::vec3(v.x, v.y, v.z);
}

void Camera::set_up(glm::vec3 v) {
    up = glm::vec3(v.x, v.y, v.z);
}

void Camera::set_right(glm::vec3 v) {
    right = glm::vec3(v.x, v.y, v.z);
}

void Camera::set_lookAt(glm::vec3 v) {
    lookAt = glm::vec3(v.x, v.y, v.z);
}

void Camera::print() {
    std::cout << "Camera:\n";
    std::cout << "- Location: {";
    std::cout << loc.x << " " << loc.y << " " << loc.z << "}\n";
    std::cout << "- Up: {"; 
    std::cout << up.x << " " << up.y << " " << up.z << "}\n";
    std::cout << "- Right: {";
    std::cout << right.x << " " << right.y << " " << right.z << "}\n";
    std::cout << "- Look at: {";
    std::cout << lookAt.x << " " << lookAt.y << " " << lookAt.z << "}\n";
}