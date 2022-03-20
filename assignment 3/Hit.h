#ifndef HIT_H
#define HIT_H

#include "Object.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

struct Hit {
    Hit(Point3 p, double t = -1, double u = 0, double v = 0) : p(p), t(t), u(u), v(v) {}
    // Hit& operator=(const Hit &rhs){
    //     if(this == &rhs){
    //         return *this;
    //     }
    //     p = rhs.p;
    //     t = rhs.t;
    //     u = rhs.u;
    //     v = rhs.v;
    //     w = rhs.w;
    //     return *this;
    // }
    Point3 p;
    double t;
    double u;
    double v;
};

#endif