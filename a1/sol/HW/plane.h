#ifndef PLANEH
#define PLANEH



#include "hitable.h"

//plane position:
//a   b
//d   c
class plane: public hitable{
    public:
        plane(){}
        plane(vec3 aa, vec3 bb, vec3 cc, vec3 dd, material *m) : a(aa), b(bb), c(cc), d(dd),  mat_ptr(m){};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec/*, vec3* barys*/) const;
        vec3 a;
        vec3 b;
        vec3 c;
        vec3 d;
material *mat_ptr;
};

//hit with two triangles
bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    //normal to the plane
    vec3 n = cross(b-a, b-c);
    //distance of ray origin to the plane
    float t = dot(n, b-r.origin())/dot(n, r.direction());
    //point of intersection
    vec3 p = r.origin()+r.direction()*t;

    //first triangle : a, b, d
    float cross_prod1 = dot(cross(b-a, p-a), n);
    float cross_prod2 = dot(cross(d-b, p-b), n);
    float cross_prod3 = dot(cross(a-d, p-d), n);
    if((cross_prod1>=0 && cross_prod2>=0 && cross_prod3>=0) || (cross_prod1<0 && cross_prod2<0 && cross_prod3<0)){
         //barycentric coordinates at point p.
         if(t<t_max && t>t_min){
           rec.t = t;
           //rec.p = r.point_at_parameter(rec.t);
           rec.p = p;
           rec.normal = n;rec.mat_ptr = mat_ptr;
/*
           float alpha = (length(cross(tr.b - p, tr.d - p)) / 2) / (length(cross( tr.b - tr.a,  tr.d - tr.a)) / 2);
           float beta = (length(cross(tr.d - p, tr.a - p)) / 2) / (length(cross(tr.b - tr.a, tr.d - tr.a )) / 2);
           float garmma = (length(cross( tr.a - p, tr.b - p)) / 2) / (length(cross(tr.b - tr.a, tr.d - tr.a)) / 2);
           bary->y = alpha;
           bary->x = beta;
           bary->z = garmma;
*/
           return true;
         }
    }
    //second triangle : d, b, c
    cross_prod1 = dot(cross(b-d, p-d), n);
    cross_prod2 = dot(cross(c-b, p-b), n);
    cross_prod3 = dot(cross(d-c, p-c), n);
    if((cross_prod1>=0 && cross_prod2>=0 && cross_prod3>=0) || (cross_prod1<0 && cross_prod2<0 && cross_prod3<0)){
         //barycentric coordinates at point p.
         if(t<t_max && t>t_min){
           rec.t = t;
           rec.p = p;
           rec.normal = n;rec.mat_ptr = mat_ptr;
/*
           float alpha = (length(cross(tr.b - p, tr.c - p)) / 2) / (length(cross( tr.b - tr.d,  tr.c - tr.d)) / 2);
           float beta = (length(cross(tr.c - p, tr.d - p)) / 2) / (length(cross(tr.b - tr.d, tr.c - tr.d )) / 2);
           float garmma = (length(cross( tr.d - p, tr.b - p)) / 2) / (length(cross(tr.b - tr.d, tr.c - tr.d)) / 2);
           bary->y = alpha;
           bary->x = beta;
           bary->z = garmma;
*/
           return true;
         }
    }
    return false;
}

class xy_rect: public hitable  {
    public:
        xy_rect() {}
        xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
        virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;

        material  *mp;
        float x0, x1, y0, y1, k;
};

class xz_rect: public hitable  {
    public:
        xz_rect() {}
        xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;

        material  *mp;
        float x0, x1, z0, z1, k;
};

class yz_rect: public hitable  {
    public:
        yz_rect() {}
        yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;

        material  *mp;
        float y0, y1, z0, z1, k;
};




bool xy_rect::hit(const ray& r, float t0, float t1, hit_record& rec) const {
    float t = (k-r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0, 0, 1);
    return true;
}


bool xz_rect::hit(const ray& r, float t0, float t1, hit_record& rec) const {
    float t = (k-r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0, 1, 0);
    return true;
}

#endif
