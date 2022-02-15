#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

class Sphere : public Object{
    public:
        Sphere(Point3 orig, double r) : origin(orig), radius(r), mat(Material(Color(0,0,0), Color(1,1,1), 0.1, 1.0, 1.0, 32.0)){}
        Sphere(Point3 orig, double r, Material c) : origin(orig), radius(r), mat(c){}
        virtual double hit(const Ray& r) const {
            Vector3 oc = r.getOrigin() - origin;
            auto a = dot(r.getDirection(), r.getDirection());
            auto b = 2.0 * dot(oc, r.getDirection());
            auto c = dot(oc, oc) - radius*radius;
            auto discriminant = b*b - 4*a*c;
            if (discriminant < 0) {
                return -1.0;
            } else {
                return (-b - sqrt(discriminant) ) / (2.0*a);
            }
        }
        virtual Material getMaterial() const {return mat;}
        virtual Point3 getPos() const {return origin;}
    private:
        Point3 origin;
        double radius;
        Material mat;
};


#endif