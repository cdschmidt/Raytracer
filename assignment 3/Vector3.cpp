#include "Vector3.h"


Vector3::Vector3(){
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(const Vector3 &vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

Vector3::Vector3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3::X() const 
{
    return x;
}

double Vector3::Y() const 
{
    return y;
}

double Vector3::Z() const 
{
    return z;
}

double Vector3::dot(Vector3 vec)
{
    return x * vec.x + y * vec.y + z * vec.z;
}

Vector3 Vector3::cross(Vector3 vec)
{
    return Vector3( y * vec.z - z * vec.y,
                    z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x);
}

void Vector3::add(Vector3 vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

void Vector3::subtract(Vector3 vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}

void Vector3::normalize()
{
    x /= length();
    y /= length();
    z /= length();
}

Vector3 Vector3::normalized()
{
    return *this / length();
}

void Vector3::multiply(double num)
{
    x *= num;
    y *= num;
    z *= num;
}

void Vector3::divide(double num){
    x /= num;
    y /= num;
    z /= num;
}

double Vector3::length_squared() const
{
    return x*x + y*y + z*z;
}

double Vector3::length() const
{
    return sqrt(length_squared());
}

Vector3 Vector3::operator+(const Vector3& rhs)
{
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3& rhs)
{
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator-()
{
    return Vector3(-x,-y,-z);
}

Vector3 Vector3::operator/(const double num)
{
    return Vector3(x / num, y / num, z / num);
}

void Vector3::print() const
{
    std::cout << x << ' ' << y << ' ' << z << std::endl;
}

