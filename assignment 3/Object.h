#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vector3.h"
#include "Material.h"

class Object{
    public:
        virtual double hit(const Ray& r) const {return false;}
        virtual Material getMaterial() const {return Material(Color(0,0,0), Color(1,1,1), 0.1, 1.0, 1.0, 32.0);}
        virtual Vector3 getNormal(double t, const Ray& r) const {return Vector3(0,0,0);}
};

#endif