#ifndef TRIH
#define TRIH

#include "hitable.h"

class tri: public hitable{
	public:
		tri(){}
		tri(vec3 a,vec3 b,vec3 c) : A(a), B(b), C(c) {};
		virtual bool hit (const ray& r, float tmin, 
							float tmax, hit_record& rec) const;
		vec3 A; 
		vec3 B; 
		vec3 C; 
};

bool tri::hit (const ray& r, float t_min, float t_max, hit_record& rec) const{
	vec3 n = cross(B-A,B-C);
	float t = dot(n, (B - r.origin()))/dot(n, r.direction());
   	vec3 p = r.origin() + r.direction() * t;

	float val1 = dot(cross((B - A), (p - A)), n);
    	float val2 = dot(cross((C - B), (p - B)), n);
    	float val3 = dot(cross((A - C), (p - C)), n);

 	if(((val1 >= 0) && (val2 >= 0) && (val3 >= 0))||
      		((val1 < 0) && (val2 < 0) && (val3 < 0))){	
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = p;
			rec.normal = n;
			return true;
		}
	}
	return false;
}

#endif
