#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Plane {

public:
    Plane();
    Plane(glm::vec3 n, glm::vec3 c, double dis, double a, double dif, std::vector<glm::vec4> transf);
    glm::vec3 get_normal() const {return normal;}
    glm::vec3 get_rgb() const {return rgb;}
    std::vector<glm::vec4> get_transform() const {return transform;}
    double get_distance() const {return distance;}
    double get_ambient() const {return ambient;}
    double get_diffuse() const {return diffuse;}

    void set_normal(glm::vec3 v);
    void set_rgb(glm::vec3 v);
    void set_transform(std::vector<glm::vec4> vectList);
    void set_distance(double d);
    void set_ambient(double d);
    void set_diffuse(double d);

private:
    glm::vec3 normal, rgb;
    std::vector<glm::vec4> transform;
    double distance, ambient, diffuse;
};