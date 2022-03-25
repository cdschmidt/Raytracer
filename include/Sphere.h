#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Hit.h"
#define PI 3.14159265
class Sphere : public Object{
    public:
        Sphere(Point3 orig, double r) : origin(orig), radius(r), mat(Material(Color(0,0,0), Color(1,1,1), 0.1, 1.0, 1.0, 32.0)){}
        Sphere(Point3 orig, double r, Material c) : origin(orig), radius(r), mat(c){}
        virtual Hit hit(const Ray& r) const {
            Vector3 oc = r.getOrigin() - origin;
            auto a = dot(r.getDirection(), r.getDirection());
            auto b = 2.0 * dot(oc, r.getDirection());
            auto c = dot(oc, oc) - radius*radius;
            auto discriminant = b*b - 4*a*c;
            if (discriminant < 0) {
                return Hit(Point3(0,0,0));
            } else {
                double t = (-b - sqrt(discriminant) ) / (2.0*a);
                Point3 p = r.at(t);
                Vector3 norm = (p - origin).normalized();

                double u = 0.5 + atan2(norm.x, norm.z) / (2*PI);
                double v = 0.5 - asin(-norm.y) / PI;

                return Hit(p,t,u,v);
            }
        }
        virtual Material getMaterial() const {return mat;}
        virtual Vector3 getNormal(double t, const Ray& r) const {
            Point3 surface = r.at(t);
            Vector3 N = surface - origin;
            return N;
        }
    private:
        Point3 origin;
        double radius;
        Material mat;
};


#endif