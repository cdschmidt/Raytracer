#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vector3.h"
#include "Material.h"
#include "Hit.h"

class Object{
    public:
        virtual Hit hit(const Ray& r) const {return Hit(Point3(0,0,0));}
        virtual Material getMaterial() const {return Material();}
        virtual Vector3 getNormal(double t, const Ray& r) const {return Vector3(0,0,0);}
        virtual bool HasTex() const {return false;}
};

#endif