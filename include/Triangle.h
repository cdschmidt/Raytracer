#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Hit.h"
#include "UV.h"

constexpr float kEpsilon = 1e-8;
class Triangle : public Object{
    public:
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2) : v0(vert0), v1(vert1), v2(vert2), mat(Material()), isSmoothShaded(false) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, Material c) : v0(vert0), v1(vert1), v2(vert2), mat(c), isSmoothShaded(false) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, UV uv0, UV uv1, UV uv2) : v0(vert0), v1(vert1), v2(vert2), uv0(uv0), uv1(uv1), uv2(uv2), mat(Material()), isSmoothShaded(false) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, UV uv0, UV uv1, UV uv2, Material c) : v0(vert0), v1(vert1), v2(vert2), uv0(uv0), uv1(uv1), uv2(uv2), mat(c), isSmoothShaded(false) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, Vector3 nv0, Vector3 n1, Vector3 n2) : v0(vert0), v1(vert1), v2(vert2), nv0(nv0), nv1(n1), nv2(n2), mat(Material()), isSmoothShaded(true) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, Vector3 n0, Vector3 n1, Vector3 n2, Material c) : v0(vert0), v1(vert1), v2(vert2), nv0(n0), nv1(n1), nv2(n2), mat(c), isSmoothShaded(true) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, UV uv0, UV uv1, UV uv2, Vector3 n0, Vector3 n1, Vector3 n2) : v0(vert0), v1(vert1), v2(vert2), uv0(uv0), uv1(uv1), uv2(uv2), nv0(n0), nv1(n1), nv2(n2), mat(Material()), isSmoothShaded(true) {}
        Triangle(Point3 vert0, Point3 vert1, Point3 vert2, UV uv0, UV uv1, UV uv2, Vector3 n0, Vector3 n1, Vector3 n2, Material c) : v0(vert0), v1(vert1), v2(vert2), uv0(uv0), uv1(uv1), uv2(uv2), nv0(n0), nv1(n1), nv2(n2), mat(c), isSmoothShaded(true) {}
        
        
        virtual Hit hit(const Ray& r) const {
            double a, b, y;
            Vector3 orig = r.getOrigin();
            Vector3 dir = r.getDirection();
            // compute plane's normal
            Vector3 v0v1 = v1 - v0; 
            Vector3 v0v2 = v2 - v0; 
            // no need to normalize
            Vector3 N = cross(v0v1, v0v2); // N 
            float area2 = N.length(); 

            // Step 1: finding P
            Hit badHit(Point3(0,0,0));
            // check if ray and plane are parallel ?
            float NdotRayDirection = dot(N, dir); 
            if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
                return badHit; // they are parallel so they don't intersect ! 

            // compute d parameter using equation 2
            float d = dot(-N, v0); 

            // compute t (equation 3)
            double t = -(dot(N, orig) + d) / NdotRayDirection; 

            // check if the triangle is in behind the ray
            if (t < 0.000001) return badHit; // the triangle is behind 

            // compute the intersection point using equation 1
            Vector3 P = orig + t * dir; 

            // Step 2: inside-outside test
            Vector3 C; // vector perpendicular to triangle's plane 

            // edge 0
            Vector3 edge0 = v1 - v0; 
            Vector3 vp0 = P - v0;
            C = cross(edge0, vp0); 
            if (dot(N, C) < 0) return badHit; // P is on the right side 

            // edge 1
            Vector3 edge1 = v2 - v1; 
            Vector3 vp1 = P - v1; 
            C = cross(edge1, vp1);
            a = C.length() / area2; 
            if (dot(N, C) < 0)  return badHit; // P is on the right side 

            // edge 2
            Vector3 edge2 = v0 - v2; 
            Vector3 vp2 = P - v2; 
            C = cross(edge2, vp2);
            b = C.length() / area2;
            y = 1 - a - b;
            if (dot(N, C) < 0) return badHit; // P is on the right side; 
            Point3 p = r.at(t);
            Hit goodHit(p, t);
            double u, v;
            u = a * uv0.u + b * uv1.u + y * uv2.u;
            v = a * uv0.v + b * uv1.v + y * uv2.v;
            goodHit.u = u;
            goodHit.v = v;
            return goodHit;


        }
        virtual Material getMaterial() const {return mat;}
        virtual Point3 getNormal(double t, const Ray& r) const {
            if(!isSmoothShaded){
                Vector3 v0v1 = v1 - v0; 
                Vector3 v0v2 = v2 - v0; 
                Vector3 N = cross(v0v1, v0v2);
                return N;
            }

            double a, b, y;
            Vector3 orig = r.getOrigin();
            Vector3 dir = r.getDirection();
            // compute plane's normal
            Vector3 v0v1 = v1 - v0; 
            Vector3 v0v2 = v2 - v0; 
            // no need to normalize
            Vector3 N = cross(v0v1, v0v2); // N 
            float area2 = N.length();
            Vector3 P = orig + t * dir; 

            // Step 2: inside-outside test
            Vector3 C;

            // edge 0
            Vector3 edge0 = v1 - v0; 
            Vector3 vp0 = P - v0; 
            C = cross(edge0, vp0);

            // edge 1
            Vector3 edge1 = v2 - v1; 
            Vector3 vp1 = P - v1; 
            C = cross(edge1, vp1);
            a = C.length() / area2;

            // edge 2
            Vector3 edge2 = v0 - v2; 
            Vector3 vp2 = P - v2; 
            C = cross(edge2, vp2);
            b = C.length() / area2; 
            y = 1 - a - b; 

            Vector3 norm = (a*nv0+b*nv1+y*nv2).normalized();
            return norm;
        }
        virtual bool HasTex() const {
            return hasTex;
        }
        void setTex(bool tex) {
            hasTex = tex;
        }
    private:
        Point3 v0;
        Point3 v1;
        Point3 v2;

        Vector3 nv0;
        Vector3 nv1;
        Vector3 nv2;

        UV uv0;
        UV uv1;
        UV uv2;

        Material mat;

        bool isSmoothShaded;
        bool hasTex;
};


#endif