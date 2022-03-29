#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray {
    public:
        Ray() {}
        Ray(const Point3& orig, const Vector3& dir) : origin(orig), direction(dir), inObject(false) {}
        Ray(const Point3& orig, const Vector3& dir, bool inObject) : origin(orig), direction(dir), inObject(inObject) {}

        Point3 getOrigin() const {return origin;}
        Vector3 getDirection() const {return direction;}

        Point3 at(double t) const {
            return origin + t*direction;
        }

        bool InObject() const{
            return inObject;
        }

        void switchInObject() {
            inObject = !inObject;
        }

    private:
        Point3 origin;
        Vector3 direction;
        bool inObject;
};

#endif