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
#include "ReflectRefract.h"
#include "Intersection.h"
#include "BVHNode.h"
#include "stb_image_write.h"


using namespace std;

/* print help message */
void print_help() {
    cout << "Usage: raytrace render <input_filename> <width> <height> [-fresnel] [-ss=N] [-gi]\n";
    cout << "       raytrace sceneinfo <input_filename>\n";
    cout << "       raytrace pixelray <input_filename> <width> <height> <x> <y>\n";
    cout << "       raytrace firsthit <input_filename> <width> <height> <x> <y>\n";
    cout << "       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]\n";
    cout << "       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]\n";
    cout << "       raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]\n";
}

void check_alt_args(int argc, char **argv, bool *fresnel, bool *altBRDF, int *superSample, bool *sds, bool *gi){
    int foundNdx;
    for (int i = 0; i < argc; i++) {
        string arg(argv[i]);
        if (arg.compare("-fresnel") == 0) {
            *fresnel = true;
        }
        if (arg.find("-ss=") != string::npos) {
            foundNdx = arg.find("-ss=");
            *superSample = stoi(arg.substr(foundNdx + 4, string::npos));
        }
        if (arg.compare("-altbrdf") == 0) {
            *altBRDF = true;
        }
        if (arg.compare("-sds") == 0) {
            *sds = true;
        }
        if (arg.compare("-gi") == 0) {
            *gi = true;
        }
    }
}


int main(int argc, char **argv) {

    int width, height, inX, inY, minNdx, numChannels, ssArg = 1;
    char *filename;
    vector<Light*> lights;
    vector<GeomObj*> objList, leftList, rightList, treeHits;
    vector<float> tValues;
    glm::vec3 color, objColor;
    glm::vec4 colorVec4;
    Camera cam;
    Ray* ray;
    float t;
    bool parsedFile, useAltBRDF = false, useFresnel = false, useSDS = false, useGI = false;
    string altArg;
    Intersection intersectObj;
    OptionalArgs optionalArgs = OptionalArgs();

    cout << std::setprecision(4);
    cout << std::setiosflags(std::ios::fixed);
    srand(time(NULL));

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
                float Us, Vs;

                if (argc > 5) {
                    check_alt_args(argc, argv, &useFresnel, &useAltBRDF, &ssArg, &useSDS, &useGI);
                    optionalArgs = OptionalArgs(false, useAltBRDF, useFresnel, useGI, 128, 2);
                }

                if (useSDS) {
                    BVHNode* tree = new BVHNode();
                    cout << "orignal list size: " << objList.size() << endl;
                    leftList = left_half(objList);
                    cout << "lefthalf list size: " << leftList.size() << endl;
                    rightList = right_half(objList);
                    cout << "righthalf list size: " << rightList.size() << endl;

                    tree->recursive_tree_build(objList, 0);

                }
                

                unsigned char *data = new unsigned char[width * height * numChannels];
                unsigned char red, green, blue;


                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        color = glm::vec3(0, 0, 0);
                        for (int n = 0; n < ssArg; n++) {
                            for (int m = 0; m < ssArg; m++) {
                                Us = -0.5f + (x + 0.5f + (-0.5f + (m + 0.5f)/ssArg))/width;
                                Vs = -0.5f + (y + 0.5f + (-0.5f + (n + 0.5f)/ssArg))/height;
                                ray = create_cam_ray(cam, width, height, Us, Vs);

                                // -1 means no hits
                                intersectObj = first_hit(*ray, objList, &t);
                                minNdx = intersectObj.get_hitNdx();
                                if (minNdx != -1) {
                                    color += raytrace(ray->get_pt(), ray->get_direction(), &t, objList, lights, 6, "Primary", optionalArgs);
                                }
                                
                            }
                        }

                        color = color/((float)ssArg * (float)ssArg);

                        red = (unsigned char) std::round(glm::min(1.0f, color.x) * 255);
                        green = (unsigned char) std::round(glm::min(1.0f, color.y) * 255);
                        blue = (unsigned char) std::round(glm::min(1.0f, color.z) * 255);

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
                intersectObj = first_hit(*ray, objList, &t);
                minNdx = intersectObj.get_hitNdx();
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
                intersectObj = first_hit(*ray, objList, &t);
                minNdx = intersectObj.get_hitNdx();
                if (minNdx != -1) {
                    Ray transformedRay = intersectObj.get_transformedRay();
                    glm::vec3 intersectionPt = transformedRay.get_pt() + t * transformedRay.get_direction(); 
                    glm::vec3 objNormal = objList[minNdx]->get_normal(intersectionPt);
                    glm::mat4 normalMatrix = glm::transpose(objList[minNdx]->get_inverseMatrix());
                    glm::vec4 normalTransposed = normalMatrix * glm::vec4(objNormal.x, objNormal.y, objNormal.z, 0.0f);
                    objNormal = glm::vec3(normalTransposed.x, normalTransposed.y, normalTransposed.z);
                    cout << "T = " << t << endl;
                    cout << "Object Type: " << objList[minNdx]->get_type() << endl;
                    if (altArg.compare("-altbrdf") == 0) {
                        cout << "BRDF: Alternate" << endl;
                        color = cook_torrance(lights, objList[minNdx], *ray, t, objList, false);
                    }
                    else {
                        cout << "BRDF: Blinn-Phong" << endl;
                        color = blinn_phong(lights, objList[minNdx], *ray, t, objNormal, objList, false);
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
            else if (mode.compare("pixeltrace") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);
                
                optionalArgs.set_printMode(true);
                if (argc > 7) {
                    altArg = string(argv[7]);
                    check_alt_args(argc, argv, &useFresnel, &useAltBRDF, &ssArg, &useSDS, &useGI);
                    optionalArgs = OptionalArgs(true, useAltBRDF, useFresnel, useGI, 0, 0);
                }

                ray = create_cam_ray(cam, width, height, inX, inY);
                raytrace(ray->get_pt(), ray->get_direction(), &t, objList, lights, 6, "Primary", optionalArgs);

            }
            else if (mode.compare("printrays") == 0) {
                width = atoi(argv[3]);
                height = atoi(argv[4]);
                inX = atoi(argv[5]);
                inY = atoi(argv[6]);

                optionalArgs.set_printMode(true);
                if (argc > 7) {
                    check_alt_args(argc, argv, &useFresnel, &useAltBRDF, &ssArg, &useSDS, &useGI);
                    optionalArgs = OptionalArgs(true, useAltBRDF, useFresnel, useGI, 0, 0);
                }

                ray = create_cam_ray(cam, width, height, inX, inY);
                color = raytrace(ray->get_pt(), ray->get_direction(), &t, objList, lights, 6, "Primary", optionalArgs);

                cout << "Pixel: [" << inX << ", " << inY << "] Color: (";
                cout << std::round(glm::min(1.0f, color.x) * 255) << ", ";
                cout << std::round(glm::min(1.0f, color.y) * 255) << ", "; 
                cout << std::round(glm::min(1.0f, color.z) * 255) << ")\n";
            }
        }
        else {
            cout << "Couldn't open " << filename << endl;
        }
    }
}