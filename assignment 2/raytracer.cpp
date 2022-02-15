#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Material.h"
#include "Lights.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.14159265

Color bkgcolor;
Point3 lightPos(5,5,0);
std::vector<Light> sceneLights;

void OutputColor(std::ofstream& output_stream, const Color& color){
    int ir = static_cast<int>(255 * color.X());
    int ig = static_cast<int>(255 * color.Y());
    int ib = static_cast<int>(255 * color.Z());
    output_stream << ir << ' ' << ig << ' ' << ib << '\n';
}

Color shade_ray(const Ray& r, const double t, const Object* object, const std::vector<Object*>& objects){
    Point3 surface = r.at(t);
    Vector3 lighting(0,0,0);
    double lightIntensity = .4;

    //amibent
    Vector3 ambiantLight = object->getMaterial().ka * object->getMaterial().color;

    for(auto light : sceneLights){
        //diffuse
        double diffuseK = object->getMaterial().kd;
        Vector3 lightdir(0,0,0);
        if(light.type == 0){
            lightdir = (light.posOrDir - surface).normalized();
        }
        else{
            lightdir = -light.posOrDir.normalized();
        }
        
        Point3 objectPos = object->getPos();
        Vector3 normal = (surface - objectPos).normalized();
        double diff = std::min(std::max(dot(normal, lightdir), 0.0), 1.0);
        Vector3 diffuse = diffuseK * diff * object->getMaterial().color;

        //specular
        double specK = object->getMaterial().ks;
        // Vector3 reflectDir = -lightdir - 2.0 * dot(normal, -lightdir) * normal;
        // float spec = std::pow(std::max(dot(-r.getDirection(), reflectDir), 0.0), 32);
        Vector3 halfwayDir = (lightdir + -r.getDirection()).normalized();
        float spec = std::pow(std::max(dot(normal, halfwayDir), 0.0), object->getMaterial().n);
        Vector3 specular = specK * spec * object->getMaterial().sMat;


        //shadow
        double shadow = 1;
        Ray shadowRay(surface, lightdir);
        for(Object* object : objects){
            double t = object->hit(shadowRay);
            Vector3 rayToObject = shadowRay.at(t) - surface;
            if(t > 0.0 && (rayToObject.length() < (light.posOrDir - surface).length() || light.type == 1)){
                shadow = 0;
            }
        }   
        //clamp
        lighting = lighting + shadow * lightIntensity * (diffuse + specular);
    }
    lighting = lighting + ambiantLight;
    lighting.x = std::min(1.0, lighting.x);
    lighting.y = std::min(1.0, lighting.y);
    lighting.z = std::min(1.0, lighting.z);
    return lighting;
}

Color trace_ray(const Ray& r, const std::vector<Object*>& objects) {
    for(Object* object : objects){
        double t = object->hit(r);
        if(t > 0.0){
            return shade_ray(r, t, object, objects);
        }
    }
    
    return bkgcolor;
}
 
int main(int argc, char *argv[]){
    std::vector<Object*> objects;
    int width;
    int height;
    double vfov;
    Point3 eye;
    Vector3 viewdir;
    Vector3 updir;
    std::queue<Material> mtlcolors;


    std::ifstream inputFile;
    std::string line;
    //If file is passsed into command line argument then opens the file and gets the width and height
    if(argc > 1){
        std::string inputFileName = argv[1];
        std::ifstream inputFile;
        inputFile.open(inputFileName);
        if(inputFile.is_open()){
            std::string line;
            while(std::getline(inputFile, line)){
                if(line == ""){
                    continue;
                }
                std::stringstream ss(line);
                std::string word;
                ss >> word;
                if(word == "imsize"){

                    ss >> width;
                    ss >> height;
                }
                else if(word == "eye"){

                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    eye = Point3(std::stod(x),std::stod(y),std::stod(z));
                }
                else if(word == "viewdir"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    viewdir = Vector3(std::stod(x),std::stod(y),std::stod(z)).normalized();
                }
                else if(word == "updir"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    updir = Vector3(std::stod(x),std::stod(y),std::stod(z)).normalized();
                }
                else if(word == "vfov"){
                    std::string deg;
                    ss >> deg;
                    vfov = std::stod(deg);
                }
                else if(word == "sphere"){
                    std::string x;
                    std::string y;
                    std::string z;
                    std::string rad;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    ss >> rad;

                    Point3 spherePos = Point3(std::stod(x),std::stod(y),std::stod(z));
                    if(mtlcolors.size() > 0){
                        Material sphereMat = mtlcolors.front();
                        mtlcolors.pop();
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad), sphereMat);
                        objects.push_back(sphere);
                    }
                    else{
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad));
                        objects.push_back(sphere);
                    }
                    
                    
                }
                else if(word == "bkgcolor"){

                    std::string r;
                    std::string g;
                    std::string b;

                    ss >> r;
                    ss >> g;
                    ss >> b;
                    bkgcolor = Color(std::stod(r),std::stod(g),std::stod(b));
                }
                else if(word == "mtlcolor"){

                    std::string r;
                    std::string g;
                    std::string b;
                    std::string sr;
                    std::string sg;
                    std::string sb;
                    std::string ka;
                    std::string kd;
                    std::string ks;
                    std::string n;

                    ss >> r;
                    ss >> g;
                    ss >> b;
                    ss >> sr;
                    ss >> sg;
                    ss >> sb;
                    ss >> ka;
                    ss >> kd;
                    ss >> ks;
                    ss >> n;

                    Color mat = Color(std::stod(r),std::stod(g),std::stod(b));
                    Color sMat = Color(std::stod(sr),std::stod(sg),std::stod(sb));
                    double Ka = std::stod(ka);
                    double Kd = std::stod(kd);
                    double Ks = std::stod(ks);
                    double N = std::stod(n);
                    Material mtl(mat, sMat, Ka, Kd, Ks, N);
                    mtlcolors.push(mtl);
                }
                else if(word == "light"){

                    std::string x;
                    std::string y;
                    std::string z;
                    std::string w;
                    std::string r;
                    std::string g;
                    std::string b;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    ss >> w;
                    ss >> r;
                    ss >> g;
                    ss >> b;
                    Vector3 pos(std::stod(x),std::stod(y),std::stod(z));
                    int type = std::stoi(w);
                    Color color = Color(std::stod(r),std::stod(g),std::stod(b));
                    Light light(pos, type, color);
                    sceneLights.push_back(light);
                }
                else{
                    std::cout << word << " is not a valid keyword" << std::endl;
                    return 0;
                }
            }
            
        }
        inputFile.close();
    }
    else{
        std::cout << "No input file provided" << std::endl;
        return 0;
    }

    if(width <= 0 || height <= 0){
        std::cout << "width or height not valid" << std::endl;
        return 0;
    }

    //Calculate correct viewport width height and orientaion for the given vfov and viewdir/updir
    double aspectRatio = double(width) / double(height);
    double focalLength = 1.0;
    double viewportHeight = 2.0 * tan((vfov/2.0) * PI / 180.0) * focalLength;
    double viewportWidth = aspectRatio * viewportHeight;
    Vector3 horizontal = viewportWidth*cross(viewdir, updir).normalized();
    Vector3 vertical = viewportHeight*cross(horizontal.normalized(), viewdir).normalized();
    Point3 lowerLeftCorner = eye - horizontal/2 - vertical/2 + viewdir*focalLength;

    std::string outputFile = argv[1];
    outputFile = outputFile.substr(0, outputFile.length()-4);
    outputFile.append(".ppm");
    //Creates an output stream.
    std::ofstream output_stream(outputFile, std::ios::out | std::ios::binary);
    //Writes header to the file.
    output_stream << "P3\n" << width << std::endl << height << std::endl << 255 << std::endl;
 
    for(uint32_t y = height-1; y > 0; y--){
        //std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for(uint32_t x = 0; x < width; x++){
            auto u = double(x) / (width-1);
            auto v = double(y) / (height-1);
            Vector3 rayDir = lowerLeftCorner + u*horizontal + v*vertical - eye;
            rayDir.normalize();
            Ray r(eye, rayDir);
            Color pixel = trace_ray(r, objects);
            OutputColor(output_stream, pixel);
            
        }
    }

    output_stream.close();
 
    return 0;
}
