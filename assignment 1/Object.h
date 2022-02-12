#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vector3.h"

class Object{
    public:
        virtual bool hit(const Ray& r) const {return false;}
        virtual Color getColor() const {return mat;}
        virtual Point3 getPos() const {return origin;}
    protected:
        Point3 origin;
        Color mat;

};

#endif