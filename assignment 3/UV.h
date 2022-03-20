#ifndef UV_H
#define UV_H

struct UV{
    UV() : u(0), v(0) {}
    UV(double u, double v) : u(u), v(v) {}

    double u, v;
};

#endif