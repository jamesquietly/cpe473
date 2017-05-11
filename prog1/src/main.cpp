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
#include "Parse.h"
#include "Camera.h"
#include "Light.h"
#include "GeomObj.h"
#include "Ray.h"
#include "Shading.h"
#include "stb_image_write.h"


using namespace std;

glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, vector<GeomObj*> objList, vector<Light*> lightList, int depth) {
    glm::vec3 color, local, reflectColor, reflectionVec, intersectionPt, objNormal;
    glm::vec3 refractionVec, refractionColor;
    glm::vec4 objColor;
    color = glm::vec3(0, 0, 0);
    int hitNdx;
    float t, epsilon, DdotN, n1, n2, localContribut;
    float reflectCoeff, refractCoeff, filterVal;
    Ray ray = Ray(p0, d);
    epsilon = 0.001f;

    if (depth > 0) {

        hitNdx = first_hit(ray, objList, &t);
        if (hitNdx != -1) {
            local = blinn_phong(lightList, objList[hitNdx], ray, t, objList);
            intersectionPt = p0 + t * d;
            objNormal = objList[hitNdx]->get_normal(intersectionPt);
            reflectionVec = ray.calc_reflection(objNormal);
            reflectColor = raytrace(intersectionPt + reflectionVec * epsilon, reflectionVec, objList, lightList, depth - 1);
            reflectCoeff = (float)objList[hitNdx]->get_reflection();
            DdotN = glm::dot(d, objNormal);
            if (DdotN >= 0) {
                n1 = 1.0f;
                n2 = objList[hitNdx]->get_ior();
            }
            else {
                n1 = objList[hitNdx]->get_ior();
                n2 = 1.0f;
                objNormal = -1.0f * objNormal;
            }
            refractionVec = ray.calc_refraction(objNormal, n1, n2);
            refractionColor = raytrace(intersectionPt + refractionVec * epsilon, refractionVec, objList, lightList, depth - 1);
            refractCoeff = (float)objList[hitNdx]->get_refraction();
            
            objColor = objList[hitNdx]->get_rgb();
            filterVal = objColor.w;
            localContribut = (1.0f - filterVal) * (1.0f - reflectCoeff);

            color += localContribut * local + reflectCoeff * reflectColor + refractCoeff * refractionColor;
        }
    }
    return color;
}

/* print help message */
void print_help() {
    cout << "Usage: raytrace render <input_filename> <width> <height> [-altbrdf]\n";
    cout << "       raytrace sceneinfo <input_filename>\n";
    cout << "       raytrace pixelray <input_filename> <width> <height> <x> <y>\n";
    cout << "       raytrace firsthit <input_filename> <width> <height> <x> <y>\n";
    cout << "       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]\n";
}


int main(int argc, char **argv) {

    int width, height, inX, inY, minNdx, numChannels;
    char *filename;
    vector<Light*> lights;
    vector<GeomObj*> objList;
    vector<float> tValues;
    glm::vec3 color, objColor;
    glm::vec4 colorVec4;
    Camera cam;
    Ray* ray;
    float t;
    bool parsedFile;
    string altArg;

    cout << std::setprecision(4);

    if (argc < 3) {
        print_help();
    }
    else {
        string mode(argv[1]);
        filename = argv[2];
        altArg = "null";

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
                    //exclude empty line at end
                    if (i != objList.size() - 1) {
                        cout << endl;
                    }
                }

            }
            else if (mode.compare("render") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                numChannels = 3;
                string outName = "output.png";

                if (argc > 5) {
                    altArg = string(argv[5]);
                }

                unsigned char *data = new unsigned char[width * height * numChannels];


                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        ray = create_cam_ray(cam, width, height, x, y);

                        unsigned char red, green, blue;

                        // -1 means no hits
                        minNdx = first_hit(*ray, objList, &t);
                        if (minNdx != -1) {
                            if(altArg.compare("-altbrdf") == 0) {
                                color = cook_torrance(lights, objList[minNdx], *ray, t, objList);
                            }
                            else {
                                //color = blinn_phong(lights, objList[minNdx], *ray, t, objList);
                                color = raytrace(ray->get_pt(), ray->get_direction(), objList, lights, 6);
                            }
                            red = (unsigned char) std::round(glm::min(1.0f, color.x) * 255);
                            green = (unsigned char) std::round(glm::min(1.0f, color.y) * 255);
                            blue = (unsigned char) std::round(glm::min(1.0f, color.z) * 255);
                        }
                        else {
                            red = 0;
                            green = 0;
                            blue = 0;
                        }
                        
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 0] = red;
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 1] = green;
                        data[(width * numChannels) * (height - 1 - y) + numChannels * x + 2] = blue;
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

                ray = create_cam_ray(cam, width, height, inX, inY);

                cout << "Pixel: [" << inX << ", " << inY << "] ";
                ray->print();
                cout << endl;
            }
            else if (mode.compare("firsthit") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);

                ray = create_cam_ray(cam, width, height, inX, inY);

                cout << "Pixel: [" << inX << ", " << inY << "] ";
                ray->print();
                cout << endl;
                
                // -1 means no hits
                minNdx = first_hit(*ray, objList, &t);
                if (minNdx != -1) {
                    cout << "T = " << t << endl;
                    cout << "Object Type: " << objList[minNdx]->get_type() << endl;
                    colorVec4 = objList[minNdx]->get_rgb();
                    cout << "Color: ";
                    cout << colorVec4.x << " " << colorVec4.y << " " << colorVec4.z << endl;
                }
                else {
                    cout << "No Hit" << endl;
                }
            }
            else if (mode.compare("pixelcolor") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);

                if (argc > 7) {
                    altArg = string(argv[7]);
                }

                ray = create_cam_ray(cam, width, height, inX, inY);

                cout << "Pixel: [" << inX << ", " << inY << "] ";
                ray->print();
                cout << endl;

                // -1 means no hits
                minNdx = first_hit(*ray, objList, &t);
                if (minNdx != -1) {
                    cout << "T = " << t << endl;
                    cout << "Object Type: " << objList[minNdx]->get_type() << endl;
                    if (altArg.compare("-altbrdf") == 0) {
                        cout << "BRDF: Alternate" << endl;
                        color = cook_torrance(lights, objList[minNdx], *ray, t, objList);
                    }
                    else {
                        cout << "BRDF: Blinn-Phong" << endl;
                        color = blinn_phong(lights, objList[minNdx], *ray, t, objList);
                    }

                    cout << "Color: (";
                    cout << std::round(glm::min(1.0f, color.x) * 255) << ", ";
                    cout << std::round(glm::min(1.0f, color.y) * 255) << ", "; 
                    cout << std::round(glm::min(1.0f, color.z) * 255) << ")" << endl;
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