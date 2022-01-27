#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3 {
    public:
        Vector3();
        Vector3(Vector3& vec);
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
        Vector3 operator*(const double num);
        Vector3 operator/(const double num);
    private:
        double x;
        double y;
        double z;
};

using Point3 = Vector3;   // 3D point
using Color = Vector3; 

#endif
