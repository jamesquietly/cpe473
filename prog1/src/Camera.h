#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    
public:
    Camera();
    Camera(glm::vec3 lo, glm::vec3 u, glm::vec3 r, glm::vec3 la);

    glm::vec3 loc, up, right, lookAt;
};