#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include <limits>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include "Lights.h"
#include "Hit.h"
#include "UV.h"


#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.14159265


Color trace_ray(const Ray& r, const std::vector<Object*>& objects, int depth);

Color bkgcolor;
Point3 lightPos(5,5,0);
std::vector<Light> sceneLights;
Point3 eye;


std::vector<std::vector<Color>> image;
bool texLoaded = false;

void ImageLoader(std::string filename) {
    image.clear();
    int max_color;
    int width, height;
    std::ifstream infile; // image variable used to read from a file
    infile.open(filename); // open the file, and associate image with it
    if(infile.fail()){ // true if filename doesn't exist
        throw "File failed to open";
    }
    std::string magic_num;
    infile >> magic_num >> width >> height >> max_color;

    image.resize(height);
    for (int i = 0; i < height; i++){
        image[i].resize(width);
    }
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned int red, green, blue;
            infile >> red >> green >> blue;
            if (!infile) {
                std::cerr << "Error reading from file around (" << y << "," << x << ")" << std::endl;
                return;
            }
            //std::cout << red << green << blue << std::endl;
            image[y][x] = Color(red, green, blue);
        }
    }
    texLoaded = true;

}


void OutputColor(std::ofstream& output_stream, const Color& color){
    int ir = static_cast<int>(255 * color.X());
    int ig = static_cast<int>(255 * color.Y());
    int ib = static_cast<int>(255 * color.Z());
    output_stream << ir << ' ' << ig << ' ' << ib << '\n';
}

Color shade_ray(const Ray& r, const Hit hit, const Object* object, const std::vector<Object*>& objects, int depth){
    Point3 surface = hit.p;
    double t = hit.t;
    Vector3 lighting(0,0,0);
    double lightIntensity = 1;

    //amibent
    Color color(0,0,0);
    double u = hit.u;
    double v = hit.v;
    if(object->HasTex()){
        int i = static_cast<int>(round(u * (image[0].size()-1)));
        int j = static_cast<int>(round(v * (image.size()-1)));
        auto red = image[j][i].x;
        auto green = image[j][i].y;
        auto blue = image[j][i].z;
        red /= 255;
        green /= 255;
        blue /= 255;
        color = Color(red,green,blue);
    }
    else{
        color = object->getMaterial().color;
    }

    Vector3 ambiantLight = object->getMaterial().ka * color;

    for(auto light : sceneLights){
        //diffuse
        double diffuseK = object->getMaterial().kd;
        Vector3 lightdir(0,0,0);
        if(light.type == 1){
            lightdir = (light.posOrDir - surface).normalized();
        }
        else{
            lightdir = -light.posOrDir.normalized();
        }
        

        Vector3 normal = object->getNormal(t, r).normalized();
        double diff = std::min(std::max(dot(normal, lightdir), 0.0), 1.0);
        Vector3 diffuse = diffuseK * diff * color;

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
            Hit hit = object->hit(shadowRay);
            Vector3 rayToObject = hit.p - surface;
            if(hit.t > 2 && (rayToObject.length() < (light.posOrDir - surface).length() || light.type == 1)){
                shadow = shadow * (1-object->getMaterial().opacity);
            }
        }   
        lighting = lighting + shadow * lightIntensity * (diffuse + specular);
        //lighting = lighting + lightIntensity * (diffuse + specular);
    }
    double ir = object->getMaterial().nt;
    double refraction_ratio = r.InObject() ? ir : (1.0/ir);
    auto cos_theta = fmin(dot(-r.getDirection(), object->getNormal(t,r)), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    double f0 = pow((object->getMaterial().nt - 1.0) / (object->getMaterial().nt + 1.0), 2);
    
    double fresnel = f0 + (1-f0) * (pow(1-sin(cos_theta),3));

    Vector3 reflected = reflect(r.getDirection().normalized(), object->getNormal(t, r).normalized()).normalized();
    Vector3 refracted = refract(-r.getDirection().normalized(), object->getNormal(t, r), refraction_ratio).normalized();

    Ray reflectRay(surface, reflected);
    Ray refractedRay(surface+refracted*.001, refracted, !r.InObject());
    lighting = lighting + ambiantLight + fresnel * trace_ray(reflectRay, objects, depth-1) + (1-fresnel) * (1-object->getMaterial().opacity) * trace_ray(refractedRay, objects, depth-1);

    
    //clamp
    lighting.x = std::min(1.0, lighting.x);
    lighting.y = std::min(1.0, lighting.y);
    lighting.z = std::min(1.0, lighting.z);
    return lighting;
}

Color trace_ray(const Ray& r, const std::vector<Object*>& objects, int depth) {
    if (depth <= 0){
        return Color(0,0,0);
    }
    double minT = std::numeric_limits<double>::max();
    Object* closestObj;
    Hit minHit(Point3(0,0,0));
    for(Object* object : objects){
        Hit hit = object->hit(r);
        if(hit.t > 0.0 && hit.t < minT){
            minT = hit.t;
            minHit = hit;
            closestObj = object;
        }
    }

    if(minT < std::numeric_limits<double>::max()){
        return shade_ray(r, minHit, closestObj, objects, depth);
    }
    
    return bkgcolor;
}

std::vector<Point3> vertexArr;
std::vector<UV> textureUVArr;
std::vector<Vector3> normalArr;
 
int main(int argc, char *argv[]){
    std::vector<Object*> objects;
    int width;
    int height;
    double vfov;

    Vector3 viewdir;
    Vector3 updir;
    Material* mtlInstance;
    mtlInstance = NULL;

    std::ifstream inputFile;
    std::string line;
    //If file is passsed into command line argument then opens the file and gets the width and height
    int lineCount = 0;
    if(argc > 1){
        std::string inputFileName = argv[1];
        std::ifstream inputFile;
        inputFile.open(inputFileName);
        if(inputFile.is_open()){
            std::string line;
            while(std::getline(inputFile, line)){
                lineCount++;
                if(line == "" || line == " "){
                    continue;
                }
                std::stringstream ss(line);
                std::string word;
                ss >> word;
                if(word == "imsize"){

                    ss >> width;
                    ss >> height;
                }
                else if(word == "f"){
                    line = line.substr(2, line.length()-2);
                    const char* w = line.c_str();
                    unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

                    if (sscanf(w, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9){
                        Point3 v0 = vertexArr[vertexIndex[0]-1];
                        Point3 v1 = vertexArr[vertexIndex[1]-1];
                        Point3 v2 = vertexArr[vertexIndex[2]-1];

                        UV uv0 = textureUVArr[uvIndex[0]-1];
                        UV uv1 = textureUVArr[uvIndex[1]-1];
                        UV uv2 = textureUVArr[uvIndex[2]-1];

                        Vector3 n0 = normalArr[normalIndex[0]-1];
                        Vector3 n1 = normalArr[normalIndex[1]-1];
                        Vector3 n2 = normalArr[normalIndex[2]-1];

                        if(mtlInstance){
                            Triangle* tri = new Triangle(v0, v1, v2, uv0, uv1, uv2, n0, n0, n0, *mtlInstance);
                            tri->setTex(texLoaded);
                            objects.push_back(tri);
                        }
                        else {
                            Triangle* tri = new Triangle(v0, v1, v2, uv0, uv1, uv2, n0, n0, n0);
                            tri->setTex(texLoaded);
                            objects.push_back(tri);
                        }
                    }
                    else if (sscanf(w, "%d/%d %d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2], &vertexIndex[3], &uvIndex[3]) == 8){
                        Point3 v0 = vertexArr[vertexIndex[0]-1];
                        Point3 v1 = vertexArr[vertexIndex[1]-1];
                        Point3 v2 = vertexArr[vertexIndex[2]-1];
                        Point3 v3 = vertexArr[vertexIndex[3]-1];

                        UV uv0 = textureUVArr[uvIndex[0]-1];
                        UV uv1 = textureUVArr[uvIndex[1]-1];
                        UV uv2 = textureUVArr[uvIndex[2]-1];
                        UV uv3 = textureUVArr[uvIndex[3]-1];

                        if(mtlInstance){
                            Triangle* tri1 = new Triangle(v0, v1, v2, uv0, uv1, uv2, *mtlInstance);
                            Triangle* tri2 = new Triangle(v0, v2, v3, uv0, uv2, uv3, *mtlInstance);
                            tri1->setTex(texLoaded);
                            tri2->setTex(texLoaded);
                            objects.push_back(tri1);
                            objects.push_back(tri2);
                        }
                        else {
                            Triangle* tri1 = new Triangle(v0, v1, v2, uv0, uv1, uv2);
                            Triangle* tri2 = new Triangle(v0, v2, v3, uv0, uv2, uv3);
                            tri1->setTex(texLoaded);
                            tri2->setTex(texLoaded);
                            objects.push_back(tri1);
                            objects.push_back(tri2);
                        }
                    }
                    else if (sscanf(w, "%d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]) == 6){
                        Point3 v0 = vertexArr[vertexIndex[0]-1];
                        Point3 v1 = vertexArr[vertexIndex[1]-1];
                        Point3 v2 = vertexArr[vertexIndex[2]-1];

                        Vector3 n0 = normalArr[normalIndex[0]-1];
                        Vector3 n1 = normalArr[normalIndex[1]-1];
                        Vector3 n2 = normalArr[normalIndex[2]-1];

                        if(mtlInstance){
                            Triangle* tri = new Triangle(v0, v1, v2, n0, n0, n0, *mtlInstance);
                            objects.push_back(tri);
                        }
                        else {
                            Triangle* tri = new Triangle(v0, v1, v2, n0, n0, n0);
                            objects.push_back(tri);
                        }
                    }
                    else if (sscanf(w, "%d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]) == 6){
                        Point3 v0 = vertexArr[vertexIndex[0]-1];
                        Point3 v1 = vertexArr[vertexIndex[1]-1];
                        Point3 v2 = vertexArr[vertexIndex[2]-1];

                        UV uv0 = textureUVArr[uvIndex[0]-1];
                        UV uv1 = textureUVArr[uvIndex[1]-1];
                        UV uv2 = textureUVArr[uvIndex[2]-1];
                        if(mtlInstance){
                            Triangle* tri = new Triangle(v0, v1, v2, uv0, uv1, uv2, *mtlInstance);
                            tri->setTex(texLoaded);
                            objects.push_back(tri);
                        }
                        else {
                            Triangle* tri = new Triangle(v0, v1, v2, uv0, uv1, uv2);
                            tri->setTex(texLoaded);
                            objects.push_back(tri);
                        }
                    }
                    else if (sscanf(w, "%d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]) == 3){
                        Point3 v0 = vertexArr[vertexIndex[0]-1];
                        Point3 v1 = vertexArr[vertexIndex[1]-1];
                        Point3 v2 = vertexArr[vertexIndex[2]-1];

                        if(mtlInstance){
                            Triangle* tri = new Triangle(v0, v1, v2, *mtlInstance);
                            objects.push_back(tri);
                        }
                        else {
                            Triangle* tri = new Triangle(v0, v1, v2);
                            objects.push_back(tri);
                        }
                    }
                    else{
                        std::cout << "face format incorrect\n";
                    }
                }
                else if(word == "v"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    Point3 vertex(std::stod(x),std::stod(y),std::stod(z));
                    vertexArr.push_back(vertex);
                }
                else if(word == "vn"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    Vector3 noraml(std::stod(x),std::stod(y),std::stod(z));
                    normalArr.push_back(noraml);
                }
                else if(word == "vt"){
                    std::string x;
                    std::string y;

                    ss >> x;
                    ss >> y;
                    UV textureUV(std::stod(x),std::stod(y));
                    textureUVArr.push_back(textureUV);
                }
                else if(word == "texture"){
                    std::string imgName;
                    ss >> imgName;
                    ImageLoader(imgName);
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
                    if(mtlInstance){
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad), *mtlInstance);
                        sphere->setTex(texLoaded);
                        objects.push_back(sphere);
                    }
                    else{
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad));
                        sphere->setTex(texLoaded);
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
                    std::string opacity;
                    std::string refraction;

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
                    ss >> opacity;
                    ss >> refraction;

                    Color mat = Color(std::stod(r),std::stod(g),std::stod(b));
                    Color sMat = Color(std::stod(sr),std::stod(sg),std::stod(sb));
                    double Ka = std::stod(ka);
                    double Kd = std::stod(kd);
                    double Ks = std::stod(ks);
                    double N = std::stod(n);
                    double op = std::stod(opacity);
                    double ref = std::stod(refraction);
                    bool hasTex = false;
                    if(image.size() > 0){
                        hasTex = true;
                    }
                    mtlInstance = new Material(mat, sMat, Ka, Kd, Ks, N, op, ref, hasTex);
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
                else if(word[0] == '#' || word[0] == 0){
                    continue;
                }
                else{
                    std::cout << word << " is not a valid keyword on line: " << lineCount << std::endl;
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
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for(uint32_t x = 0; x < width; x++){
            auto u = double(x) / (width-1);
            auto v = double(y) / (height-1);
            Vector3 rayDir = lowerLeftCorner + u*horizontal + v*vertical - eye;
            rayDir.normalize();
            Ray r(eye, rayDir);
            Color pixel = trace_ray(r, objects, 2);
            OutputColor(output_stream, pixel);
            
        }
    }

    output_stream.close();
 
    return 0;
}
