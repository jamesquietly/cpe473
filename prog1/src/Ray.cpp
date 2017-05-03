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

/* check list of t values to see if there are any hits */
int check_hit(std::vector<float> values) {
    float noHit = -1;
    int minNdx = -1;
    float minValue = FLT_MAX;

    for (int i = 0; i < values.size(); i++) {
        if (values[i] != noHit && values[i] < minValue) {
            minValue = values[i];
            minNdx = i;
        }
    }

    return minNdx;
}