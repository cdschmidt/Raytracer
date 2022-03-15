#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"

class Light {
    public:
        Light(Vector3 l, int t, Color c) : posOrDir(l), type(t), color(c) {}
    public:
        Vector3 posOrDir;
        int type;
        Color color;
};

#endif