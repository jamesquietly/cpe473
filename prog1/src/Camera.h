#include <glm/gtc/type_ptr.hpp>

class Camera {
    
public:
    Camera();
    Camera(glm::vec3 lo, glm::vec3 u, glm::vec3 r, glm::vec3 la);
    glm::vec3 get_loc() const { return loc;}
    glm::vec3 get_up() const { return up;}
    glm::vec3 get_right() const { return right;}
    glm::vec3 get_lookAt() const { return lookAt;}
    void set_loc(glm::vec3 v);
    void set_up(glm::vec3 v);
    void set_right(glm::vec3 v);
    void set_lookAt(glm::vec3 v);

private:
    glm::vec3 loc, up, right, lookAt;
};