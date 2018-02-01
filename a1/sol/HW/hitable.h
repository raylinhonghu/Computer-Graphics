#ifndef HITABLEH
#define HITABLEH 

#include "ray.h"
//#include "aabb.h"
class material;



struct hit_record
{
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hitable  {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
      // virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};
/*
bool translate::bounding_box(float t0, float t1, aabb& box) const {
    if (ptr->bounding_box(t0, t1, box)) {
        box = aabb(box.min() + offset, box.max()+offset);
        return true;
    }
    else
        return false;
}
*/
#endif



/*
#ifndef HITABLEH
#define HITABLEH
#include "float.h"
#include "ray.h"

class material;

struct hit_record {
	float t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};


class material {
	public:	
		virtual bool scatter(const ray& r_in, const hit_record& rec,
				     vec3& attenuation, ray& scattered) const = 0; 
};

vec3 random_in_unit_sphere(){
	vec3 p;
	do{
		p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
	} while (p.squared_length() >= 1.0);
	return p;
}

class hitable {
	public:
		virtual bool hit (const ray& r, float t_min, 
							float t_max, hit_record& rec) const = 0;
		
};

vec3 color(const ray& r, hitable *world, int depth){
	hit_record rec;
	if(world->hit(r, 0.001, FLT_MAX, rec)){
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
			return attenuation*color(scattered, world, depth +1);
		}
		else{
			return vec3(0,0,0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

vec3 reflect(const vec3& v, const vec3& n){
	return v - 2*dot(v,n)*n;
}


class lambertian : public material{
	public:
		lambertian(const vec3& a) : albedo(a){}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
			vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = ray (rec.p, target- rec.p);
			attenuation = albedo;
			return true;
		}
		
		vec3 albedo;
};

class metal : public material {
	public:
		metal(const vec3& a) : albedo(a){}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
			vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
			scattered = ray(rec.p, reflected);
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
		vec3 albedo;
};

#endif
*/
