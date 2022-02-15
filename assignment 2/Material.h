#ifndef Material_H
#define Material_H

#include "Vector3.h"

struct Material {
    Color mat;
    Color sMat;
    double ka;
    double kd;
    double ks;
    double n;
};

#endif