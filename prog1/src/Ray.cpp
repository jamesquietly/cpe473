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

glm::vec3 Ray::calc_reflection(glm::vec3 normal) {
    return direction - 2.0f * glm::dot(direction, normal) * normal;
}

glm::vec3 Ray::calc_refraction(glm::vec3 normal, float n1, float n2) {
    float indexFrac, DdotN, sqRoot;
    indexFrac = n1/n2;
    DdotN = glm::dot(direction, normal);
    sqRoot = glm::sqrt(1.0f - glm::pow(indexFrac, 2.0f) * (1.0f - glm::pow(DdotN, 2.0f)));
    return indexFrac * (direction - DdotN * normal) - normal * sqRoot;
}

/* create a single ray*/
Ray* create_cam_ray(Camera cam, int width, int height, int i, int j) {
    float Us, Vs, Ws;
    glm::vec3 s, u, v, w, dir, p0;
    Ray* ray;

    p0 = cam.get_loc();

    Us = (i + 0.5)/width - 0.5;
    Vs = (j + 0.5)/height - 0.5;
    Ws = -1.0f;

    u = cam.get_right();
    v = cam.get_up();
    w = -1.0f * glm::normalize(cam.get_lookAt() - p0);

    s = p0 + Us * u + Vs * v + Ws * w;

    dir = glm::normalize(s - p0);

    ray = new Ray(p0, dir);

    return ray;
}

/* create a single ray for super sample*/
Ray* create_cam_ray(Camera cam, int width, int height, float Us, float Vs) {
    float Ws;
    glm::vec3 s, u, v, w, dir, p0;
    Ray* ray;

    p0 = cam.get_loc();

    //Us = (i + 0.5)/width - 0.5;
    //Vs = (j + 0.5)/height - 0.5;
    Ws = -1.0f;

    u = cam.get_right();
    v = cam.get_up();
    w = -1.0f * glm::normalize(cam.get_lookAt() - p0);

    s = p0 + Us * u + Vs * v + Ws * w;

    dir = glm::normalize(s - p0);

    ray = new Ray(p0, dir);

    return ray;
}