#ifndef Material_H
#define Material_H

#include "Vector3.h"

struct Material {
    Material(Color col, Color spec, double a, double d, double s, double N) : color(col), sMat(spec), ka(a), kd(d), ks(s), n(N) {}
    Material(const Material &mat) {
        color = mat.color;
        sMat = mat.sMat;
        ka = mat.ka;
        kd = mat.kd;
        ks = mat.ks;
        n = mat.n;
    }
    Color color;
    Color sMat;
    double ka;
    double kd;
    double ks;
    double n;
};

#endif