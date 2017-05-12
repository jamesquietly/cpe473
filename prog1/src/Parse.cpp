#include "Parse.h"

using namespace std;

/* parse a vector within the '< >' brackets
   returns a list of doubles
*/
vector<double> parse_vect(string context, string word) {
    vector<double> vect;
    int start;
    double x;
    string tempStr;
    stringstream ss;

    start = context.find(word);
    tempStr = context.substr(start, string::npos);
    start = tempStr.find('<');
    tempStr = tempStr.substr(start, string::npos);
    //skip over '<' in the string
    start = tempStr.find('<');
    tempStr = tempStr.substr(start + 1, string::npos);

    ss.str(tempStr);
    while (ss >> x) {
        vect.push_back(x);
        if (ss.peek() == ',') {
            ss.ignore();
        }
    }

    return vect;
}

/* parses a single double from string */
double parse_double(string context, string word) {
    double result;
    int start;
    string tempStr;
    stringstream ss;

    start = context.find(word);
    tempStr = context.substr(start, string::npos);
    if (word.compare(">,") == 0) {
        start = tempStr.find(",");
        tempStr = tempStr.substr(start + 1, string::npos);
    }
    else {
        start = tempStr.find(" ");
        tempStr = tempStr.substr(start, string::npos);
    }


    ss.str(tempStr);
    ss >> result;

    return result;
}

/* parses camera string from file
   returns a Camera
*/
Camera parse_camera(vector<string> cameraList) {
    vector<double> vect;
    glm::vec3 loc, up, right, look;
    Camera cam;


    vect = parse_vect(cameraList[0], "location");
    loc = glm::vec3(vect[0], vect[1], vect[2]);
    
    vect = parse_vect(cameraList[0], "up");
    up = glm::vec3(vect[0], vect[1], vect[2]);
    
    
    vect = parse_vect(cameraList[0], "right");
    right = glm::vec3(vect[0], vect[1], vect[2]);
    

    vect = parse_vect(cameraList[0], "look_at");
    look = glm::vec3(vect[0], (float)vect[1], vect[2]);

    cam = Camera(loc, up, right, look);

    return cam;
}

/* given a list of light_sources from file,
   parses them into a list of Light pointers
*/
vector<Light*> parse_light(vector<string> lightList) {
    vector<Light*> lights;
    vector<double> vect;
    glm::vec3 loc, color;
    Light* lightObj;
    for (int i = 0; i < lightList.size(); i++) {
        vect = parse_vect(lightList[i], "light_source {");
        loc = glm::vec3(vect[0], vect[1], vect[2]);
        
        vect = parse_vect(lightList[i], "color rgb");
        color = glm::vec3(vect[0], vect[1], vect[2]);

        lightObj = new Light(loc, color);
        lights.push_back(lightObj);
    }

    return lights;
}

/* given a sphere string from file,
   parses into a sphere object
*/
Sphere* parse_sphere(string sphereList) {
    vector<Sphere*> spheres;
    vector<double> vect;
    vector<glm::vec4> transform;
    glm::vec4 color;
    glm::vec3 cen;
    Sphere* sphere;
    int pos = 0;
    string delimiter = "\n", tempStr;
    double rad, amb, diff, spec, rough, metal, ior, reflect, refrac;
    size_t found;

    found = sphereList.find("color rgbf");
    if (found != string::npos) {
        vect = parse_vect(sphereList, "color rgbf");
        color = glm::vec4(vect[0], vect[1], vect[2], vect[3]);
    }
    else {
        vect = parse_vect(sphereList, "color rgb");
        color = glm::vec4(vect[0], vect[1], vect[2], 0);
    }

    vect = parse_vect(sphereList, "sphere {");
    cen = glm::vec3(vect[0], vect[1], vect[2]);

    rad = parse_double(sphereList, ">,");

    amb = parse_double(sphereList, "ambient");

    diff = parse_double(sphereList, "diffuse");

    spec = 0;
    found = sphereList.find("specular");
    if (found != string::npos) {
        spec = parse_double(sphereList, "specular");
    }
    
    rough = 0.5;
    found = sphereList.find("roughness");
    if (found != string::npos) {
        rough = parse_double(sphereList, "roughness");
    }

    metal = 0.5;
    found = sphereList.find("metallic");
    if (found != string::npos) {
        metal = parse_double(sphereList, "metallic");
    }

    ior = 1.0;
    found = sphereList.find("ior");
    if (found != string::npos) {
        ior = parse_double(sphereList, "ior");
    }

    reflect = 0;
    found = sphereList.find("reflection");
    if (found != string::npos) {
        reflect = parse_double(sphereList, "reflection");
    }

    refrac = 0;
    found = sphereList.find("refraction");
    if (found != string::npos) {
        refrac = parse_double(sphereList, "refraction");
    }


    while ((pos = sphereList.find(delimiter)) != string::npos ) {
        tempStr = sphereList.substr(0, pos);
        if (tempStr.find("scale") != string::npos) {
            vect = parse_vect(tempStr, "scale");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], scale_t));
        }
        else if (tempStr.find("translate") != string::npos) {
            vect = parse_vect(tempStr, "translate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], translate_t));
        }
        else if (tempStr.find("rotate") != string::npos) {
            vect = parse_vect(tempStr, "rotate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], rotate_t));
        }
        sphereList.erase(0, pos + delimiter.length());
    }


    sphere = new Sphere(cen, color, rad, amb, diff, spec, rough, metal, ior, reflect, refrac, transform);

    transform.clear();
    
    return sphere;
}

/* given a plane string from file
   parses it into a plane object
*/
Plane* parse_plane(string planeList) {
    vector<Plane*> planes;
    vector<double> vect;
    vector<glm::vec4> transform;
    glm::vec4 color;
    glm::vec3 norm;
    Plane* plane;
    double dis, amb, diff, spec, rough, metal, ior, reflect, refrac;
    int pos = 0;
    string tempStr, delimiter = "\n";
    size_t found;
    

    vect = parse_vect(planeList, "plane {");
    norm = glm::vec3(vect[0], vect[1], vect[2]);
    
    found = planeList.find("color rgbf");
    if (found != string::npos) {
        vect = parse_vect(planeList, "color rgbf");
        color = glm::vec4(vect[0], vect[1], vect[2], vect[3]);
    }
    else {
        vect = parse_vect(planeList, "color rgb");
        color = glm::vec4(vect[0], vect[1], vect[2], 0);
    }

    dis = parse_double(planeList, ">,");

    amb = parse_double(planeList, "ambient");

    diff = parse_double(planeList, "diffuse");

    spec = 0;
    found = planeList.find("specular");
    if (found != string::npos) {
        spec = parse_double(planeList, "specular");
    }

    rough = 0.5;
    found = planeList.find("roughness");
    if (found != string::npos) {
        rough = parse_double(planeList, "roughness");
    }

    metal = 0.5;
    found = planeList.find("metallic");
    if (found != string::npos) {
        metal = parse_double(planeList, "metallic");
    }

    ior = 1.0;
    found = planeList.find("ior");
    if (found != string::npos) {
        ior = parse_double(planeList, "ior");
    }

    reflect = 0;
    found = planeList.find("reflection");
    if (found != string::npos) {
        reflect = parse_double(planeList, "reflection");
    }

    refrac = 0;
    found = planeList.find("refraction");
    if (found != string::npos) {
        refrac = parse_double(planeList, "refraction");
    }


    while ((pos = planeList.find(delimiter)) != string::npos ) {
        tempStr = planeList.substr(0, pos);
        if (tempStr.find("scale") != string::npos) {
            vect = parse_vect(tempStr, "scale");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], scale_t));
        }
        else if (tempStr.find("translate") != string::npos) {
            vect = parse_vect(tempStr, "translate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], translate_t));
        }
        else if (tempStr.find("rotate") != string::npos) {
            vect = parse_vect(tempStr, "rotate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], rotate_t));
        }
        planeList.erase(0, pos + delimiter.length());
    }

    plane = new Plane(norm, color, dis, amb, diff, spec, rough, metal, ior, reflect, refrac, transform);

    transform.clear();

    return plane;
}

Triangle* parse_triangle(string triangleList) {
    Triangle* triangle;
    vector<double> vect;
    glm::vec4 color;
    glm::vec3 pt1, pt2, pt3;
    vector<glm::vec4> transform;
    int pos;
    string subStr, tempStr, delimiter = "\n";
    double amb, diff, spec, rough, metal, ior, reflect, refrac;
    size_t found;

    vect = parse_vect(triangleList, "triangle {");
    pt1 = glm::vec3(vect[0], vect[1], vect[2]);

    vect = parse_vect(triangleList, ">,");
    pt2 = glm::vec3(vect[0], vect[1], vect[2]);

    pos = triangleList.find(">,");
    subStr = triangleList.substr(pos + 2, string::npos);
    vect = parse_vect(subStr, ">,");
    pt3 = glm::vec3(vect[0], vect[1], vect[2]);

    found = triangleList.find("color rgbf");
    if (found != string::npos) {
        vect = parse_vect(triangleList, "color rgbf");
        color = glm::vec4(vect[0], vect[1], vect[2], vect[3]);
    }
    else {
        vect = parse_vect(triangleList, "color rgb");
        color = glm::vec4(vect[0], vect[1], vect[2], 0);
    }

    amb = parse_double(triangleList, "ambient");

    diff = parse_double(triangleList, "diffuse");

    spec = 0;
    found = triangleList.find("specular");
    if (found != string::npos) {
        spec = parse_double(triangleList, "specular");
    }

    rough = 0.5;
    found = triangleList.find("roughness");
    if (found != string::npos) {
        rough = parse_double(triangleList, "roughness");
    }

    metal = 0.5;
    found = triangleList.find("metallic");
    if (found != string::npos) {
        metal = parse_double(triangleList, "metallic");
    }

    ior = 1.0;
    found = triangleList.find("ior");
    if (found != string::npos) {
        ior = parse_double(triangleList, "ior");
    }

    reflect = 0;
    found = triangleList.find("reflection");
    if (found != string::npos) {
        reflect = parse_double(triangleList, "reflection");
    }

    refrac = 0;
    found = triangleList.find("refraction");
    if (found != string::npos) {
        refrac = parse_double(triangleList, "refraction");
    }

    pos = 0;
    while ((pos = triangleList.find(delimiter)) != string::npos ) {
        tempStr = triangleList.substr(0, pos);
        if (tempStr.find("scale") != string::npos) {
            vect = parse_vect(tempStr, "scale");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], scale_t));
        }
        else if (tempStr.find("translate") != string::npos) {
            vect = parse_vect(tempStr, "translate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], translate_t));
        }
        else if (tempStr.find("rotate") != string::npos) {
            vect = parse_vect(tempStr, "rotate");
            transform.push_back(glm::vec4(vect[0], vect[1], vect[2], rotate_t));
        }
        triangleList.erase(0, pos + delimiter.length());
    }

    triangle = new Triangle(pt1, pt2, pt3, color, amb, diff, spec, rough, metal, ior, reflect, refrac, transform);

    return triangle;

}

/* parse pov files
   returns true if we can find file
   else returns false
*/
bool parse_objects(char *filename, Camera *cameraObj, vector<Light*> *lights, vector<GeomObj*> *oList) 
{
    fstream f;
    size_t findStr;
    objType t;
    string tempStr = "";
    vector<string> cameraVec, lightVec, sphereVec, planeVec, triangleVec, boxVec, coneVec;
    vector<Sphere*> sphereList;
    vector<Plane*> planeList;
    Sphere* sObj;
    Plane* planeObj;
    Triangle* triObj;
    bool result;

    f.open(filename, fstream::in);
    if (f.is_open()) {
        for (string line; getline(f, line); ) {

            findStr = line.find("//");
            if (findStr != string::npos) {
                t = comment_t;
            }

            findStr = line.find("camera");
            if (findStr != string::npos) {
                t = camera_t;
            }

            findStr = line.find("light");
            if (findStr != string::npos) {
                t = light_t;
            }

            findStr = line.find("sphere");
            if (findStr != string::npos) {
                t = sphere_t;
            }

            findStr = line.find("plane");
            if (findStr != string::npos) {
                t = plane_t;
            }

            findStr = line.find("triangle");
            if (findStr != string::npos) {
                t = triangle_t;
            }

            findStr = line.find("box");
            if (findStr != string::npos) {
                t = box_t;
            }

            findStr = line.find("cone");
            if (findStr != string::npos) {
                t = cone_t;
            }

            //don't include comments
            if (t != comment_t) {
                tempStr.append(line);
                tempStr.append("\n");
            }

            if (t == light_t && tempStr.compare("\n") != 0) {
                string lightStr(tempStr);
                if (lightStr.compare("\n\n") != 0) {
                    lightVec.push_back(lightStr);
                }
                tempStr = "";
            }
            else if (t == camera_t && line.compare("}") == 0) {
                string camStr(tempStr);
                cameraVec.push_back(camStr);
                tempStr = "";
            }
            else if (t == sphere_t && (line.compare("}") == 0 || line.find("}}") != string::npos)) {
                string spStr(tempStr);
                sObj = parse_sphere(spStr);
                oList->push_back(sObj);
                tempStr = "";
            }
            else if (t == plane_t && (line.compare("}") == 0 || line.find("}}") != string::npos)) {
                string planeStr(tempStr);
                planeObj = parse_plane(planeStr);
                oList->push_back(planeObj);
                tempStr = "";
            }
            else if (t == triangle_t && (line.compare("}") == 0 || line.find("}}") != string::npos)) {
                string triStr(tempStr);
                triObj = parse_triangle(triStr);
                oList->push_back(triObj);
                tempStr = "";
            }
            else if (t == box_t && (line.compare("}") == 0 || line.find("}}") != string::npos)) {
                boxVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == cone_t && (line.compare("}") == 0 || line.find("}}") != string::npos)) {
                coneVec.push_back(tempStr);
                tempStr = "";
            }
        }

        *cameraObj = parse_camera(cameraVec);
        *lights = parse_light(lightVec);

        result = true;
    }
    else {
        result = false;
    }


    return result;
}