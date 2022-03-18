#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

constexpr float kEpsilon = 1e-8;
class Triangle : public Object{
    public:
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2) : v0(vert0), v1(vert1), v2(vert2), mat(Material(Color(1,1,1), Color(1,1,1), 0.2, 1.0, 1.0, 32.0)){}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, Material c) : v0(vert0), v1(vert1), v2(vert2), mat(c){}
        virtual double hit(const Ray& r) const {
            Vector3 orig = r.getOrigin();
            Vector3 dir = r.getDirection();
            // compute plane's normal
            Vector3 v0v1 = v1 - v0; 
            Vector3 v0v2 = v2 - v0; 
            // no need to normalize
            Vector3 N = cross(v0v1, v0v2); // N 
            float area2 = N.length(); 

            // Step 1: finding P

            // check if ray and plane are parallel ?
            float NdotRayDirection = dot(N, dir); 
            if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
                return false; // they are parallel so they don't intersect ! 

            // compute d parameter using equation 2
            float d = dot(-N, v0); 

            // compute t (equation 3)
            double t = -(dot(N, orig) + d) / NdotRayDirection; 

            // check if the triangle is in behind the ray
            if (t < 0.000001) return -1; // the triangle is behind 

            // compute the intersection point using equation 1
            Vector3 P = orig + t * dir; 

            // Step 2: inside-outside test
            Vector3 C; // vector perpendicular to triangle's plane 

            // edge 0
            Vector3 edge0 = v1 - v0; 
            Vector3 vp0 = P - v0; 
            C = cross(edge0, vp0); 
            if (dot(N, C) < 0) return -1; // P is on the right side 

            // edge 1
            Vector3 edge1 = v2 - v1; 
            Vector3 vp1 = P - v1; 
            C = cross(edge1, vp1); 
            if (dot(N, C) < 0)  return -1; // P is on the right side 

            // edge 2
            Vector3 edge2 = v0 - v2; 
            Vector3 vp2 = P - v2; 
            C = cross(edge2, vp2); 
            if (dot(N, C) < 0) return -1; // P is on the right side; 

            return t; // this ray hits the triangle 
        }
        virtual Material getMaterial() const {return mat;}
        virtual Point3 getNormal(double t, const Ray& r) const {
            if(normals.length() == 0){
                Vector3 v0v1 = v1 - v0; 
                Vector3 v0v2 = v2 - v0; 
                Vector3 N = cross(v0v1, v0v2);
                return N;
            }

            return Point3(0,0,0);
            
        }
    private:
        Point3 v0;
        Point3 v1;
        Point3 v2;
        Vector3 normals;
        Material mat;
};


#endif