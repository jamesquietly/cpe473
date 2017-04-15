/* Program 1
   Author: James Ly
*/

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cfloat>
#include <cmath>
#include "Camera.h"
#include "Light.h"
#include "GeomObj.h"
#include "Ray.h"
#include "stb_image_write.h"


enum objType {camera_t, light_t, sphere_t, plane_t, triangle_t, box_t, cone_t, comment_t};

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
    start = tempStr.find(" ");
    tempStr = tempStr.substr(start, string::npos);

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

    cam = Camera::Camera(loc, up, right, look);

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
    glm::vec3 color, cen;
    Sphere* sphere;
    int pos = 0;
    string delimiter = "\n", tempStr;
    double rad, amb, diff;



    vect = parse_vect(sphereList, "color rgb");
    color = glm::vec3(vect[0], vect[1], vect[2]);

    vect = parse_vect(sphereList, "sphere {");
    cen = glm::vec3(vect[0], vect[1], vect[2]);

    rad = parse_double(sphereList, ">,");

    amb = parse_double(sphereList, "ambient");

    diff = parse_double(sphereList, "diffuse");

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


    sphere = new Sphere(cen, color, rad, amb, diff, transform);

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
    glm::vec3 color, norm;
    Plane* plane;
    double dis, amb, diff;
    int pos = 0;
    string tempStr, delimiter = "\n";
    

    vect = parse_vect(planeList, "plane {");
    norm = glm::vec3(vect[0], vect[1], vect[2]);
    
    vect = parse_vect(planeList, "color rgb");
    color = glm::vec3(vect[0], vect[1], vect[2]);

    dis = parse_double(planeList, ">,");

    amb = parse_double(planeList, "ambient");

    diff = parse_double(planeList, "diffuse");

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

    plane = new Plane(norm, color, dis, amb, diff, transform);

    transform.clear();

    return plane;
}

/* parse pov files
   returns true if we can find file
   else returns false
*/
bool parse_objects(char *filename, Camera *cameraObj, 
                   vector<Light*> *lights, vector<GeomObj*> *oList) 
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

            findStr = line.find("trianlge");
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
                lightVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == camera_t && line.compare("}") == 0) {
                cameraVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == sphere_t && line.compare("}") == 0) {
                //sphereVec.push_back(tempStr);
                string spStr(tempStr);
                sObj = parse_sphere(spStr);
                oList->push_back(sObj);
                tempStr = "";
            }
            else if (t == plane_t && line.compare("}") == 0) {
                //planeVec.push_back(tempStr);
                string planeStr(tempStr);
                planeObj = parse_plane(planeStr);
                oList->push_back(planeObj);
                tempStr = "";
            }
            else if (t == box_t && line.compare("}") == 0) {
                boxVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == cone_t && line.compare("}") == 0) {
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

/* print help message */
void print_help() {
    cout << "Usage: raytrace render <input_filename> <width> <height>\n";
    cout << "       raytrace sceneinfo <input_filename>\n";
    cout << "       raytrace pixelray <input_filename> <width> <height> <x> <y>\n";
    cout << "       raytrace firsthit <input_filename> <width> <height> <x> <y>\n";
}

/* create a single ray*/
Ray* create_ray(Camera cam, int width, int height, int i, int j) {
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
int check_hit(vector<float> values) {
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


int main(int argc, char **argv) {

    int width, height, inX, inY, minNdx, numChannels;
    char *filename;
    vector<Light*> lights;
    vector<GeomObj*> objList;
    vector<float> tValues;
    glm::vec3 color;
    Camera cam;
    Ray* ray;
    float t;
    bool parsedFile;

    cout << std::setprecision(4);

    if (argc < 3) {
        print_help();
    }
    else {
        string mode(argv[1]);
        filename = argv[2];

        parsedFile = parse_objects(filename, &cam, &lights, &objList);

        if (parsedFile) {
            if (mode.compare("sceneinfo") == 0) {

                cam.print();
                
                cout << "\n---\n\n" << lights.size() << " light(s)\n\n";
                for (int i = 0; i < lights.size(); i++) {
                    cout << "Light[" << i << "]:\n";
                    lights[i]->print();
                    cout << endl;
                }

                cout << "---\n\n" << objList.size() << " object(s)\n\n";
                for (int i = 0; i < objList.size(); i++) {
                    cout << "Object[" << i << "]:\n";
                    objList[i]->print();
                    cout << endl;
                }

            }
            else if (mode.compare("render") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                numChannels = 3;
                string outName = "output.png";

                unsigned char *data = new unsigned char[width * height * numChannels];


                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        for (int z = 0; z < objList.size(); z++) {
                            ray = create_ray(cam, width, height, x, y);
                            t = objList[z]->intersect(*ray);
                            tValues.push_back(t);
                        }

                        unsigned char red, green, blue;

                        // -1 means no hits
                        minNdx = check_hit(tValues);
                        if (minNdx != -1) {
                            color = objList[minNdx]->get_rgb();
                            red = (unsigned char) std::round(color.x * 255);
                            green = (unsigned char) std::round(color.y * 255);
                            blue = (unsigned char) std::round(color.z * 255);
                        }
                        else {
                            red = 0;
                            green = 0;
                            blue = 0;
                        }
                        
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 0] = red;
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 1] = green;
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 2] = blue;
                        tValues.clear();
                    }
                }

                stbi_write_png(outName.c_str(), width, height, numChannels, data, width * numChannels);
                delete[] data;

            }
            else if (mode.compare("pixelray") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);

                ray = create_ray(cam, width, height, inX, inY);

                cout << "Pixel: [" << inX << ", " << inY << "] ";
                ray->print();
                cout << endl;
            }
            else if (mode.compare("firsthit") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);

                ray = create_ray(cam, width, height, inX, inY);

                for (int i = 0; i < objList.size(); i ++) {
                    t = objList[i]->intersect(*ray);
                    tValues.push_back(t);
                }

                cout << "Pixel: [" << inX << ", " << inY << "] ";
                ray->print();
                cout << endl;
                
                // -1 means no hits
                minNdx = check_hit(tValues);
                if (minNdx != -1) {
                    cout << "T = " << tValues[minNdx] << endl;
                    cout << "Object Type: " << objList[minNdx]->get_type() << endl;
                    color = objList[minNdx]->get_rgb();
                    cout << "Color: ";
                    cout << color.x << " " << color.y << " " << color.z << endl;
                }
                else {
                    cout << "No Hit" << endl;
                }
            }
        }
        else {
            cout << "Couldn't open " << filename << endl;
        }
    }
}