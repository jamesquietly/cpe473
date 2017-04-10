#include "camera.h"

Camera::Camera() {
    loc = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 0 , 0);
    right = glm::vec3(0, 0, 0);
    lookAt = glm::vec3(0, 0, 0);

}

Camera::Camera(glm::vec3 lo, glm::vec3 u, glm::vec3 r, glm::vec3 la) {
    loc = glm::vec3(lo.x, lo.y, lo.z);
    up = glm::vec3(u.x, u.y, u.z);
    right = glm::vec3(r.x, r.y, r.z);
    lookAt = glm::vec3(la.x, la.y, la.z);

}