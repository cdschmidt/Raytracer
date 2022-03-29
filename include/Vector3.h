#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3 {
    public:
        Vector3();
        Vector3(const Vector3 &vec);
        Vector3(double x, double y, double z);
        double X() const;
        double Y() const;
        double Z() const;
        double dot(Vector3 vec);
        Vector3 cross(Vector3 vec);
        void add(Vector3 vec);
        void subtract(Vector3 vec);
        void normalize();
        void multiply(double num);
        void divide(double num);
        Vector3 normalized();
        double length_squared() const;
        double length() const; 
        Vector3 operator+(const Vector3& rhs);
        Vector3 operator-(const Vector3& rhs);
        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        Vector3 operator/(const double num);
        void print() const;
    public:
        double x;
        double y;
        double z;
};

using Point3 = Vector3;   // 3D point
using Color = Vector3; 

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t*v.X(), t*v.Y(), t*v.Z());
}

inline Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs) {
    return Vector3(lhs.X()*rhs.X(), lhs.Y()*rhs.Y(), lhs.Z()*rhs.Z());
}


inline Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs) {
    return Vector3(lhs.X()+rhs.X(), lhs.Y()+rhs.Y(), lhs.Z()+rhs.Z());
}

inline Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs) {
    return Vector3(lhs.X()-rhs.X(), lhs.Y()-rhs.Y(), lhs.Z()-rhs.Z());
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.X() * v.X()
         + u.Y() * v.Y()
         + u.Z() * v.Z();
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3( u.Y() * v.Z() - u.Z() * v.Y(),
                    u.Z() * v.X() - u.X() * v.Z(),
                    u.X() * v.Y() - u.Y() * v.X());
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2*dot(v,n)*n;
}

inline Vector3 refract(const Vector3& I, const Vector3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(I, n), 1.0);
    Vector3 right_side =  etai_over_etat * (cos_theta * n - I);
    Vector3 left_side = sqrt(1-(etai_over_etat*etai_over_etat) * (1-(cos_theta*cos_theta))) * -n;
    return left_side + right_side;
}

#endif
