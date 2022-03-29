#ifndef Material_H
#define Material_H

#include "Vector3.h"

struct Material {
    Material() : color(Color(0,0,0)), sMat(Color(1,1,1)), ka(0.1), kd(1.0), ks(1.0), n(32.0), opacity(1), nt(1), hasTexture(false) {}
    Material(Color col, Color spec, double a, double d, double s, double N, double alpha, double IofR, bool tex) : color(col), sMat(spec), ka(a), kd(d), ks(s), n(N), opacity(alpha), nt(IofR), hasTexture(tex) {}
    Material(const Material &mat) {
        color = mat.color;
        sMat = mat.sMat;
        ka = mat.ka;
        kd = mat.kd;
        ks = mat.ks;
        n = mat.n;
        opacity = mat.opacity;
        nt = mat.nt;
        hasTexture = mat.hasTexture;
    }
    Color color;
    Color sMat;
    double ka;
    double kd;
    double ks;
    double n;
    double opacity;
    double nt;
    bool hasTexture;
};

#endif