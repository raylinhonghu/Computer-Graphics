#ifndef SQRH
#define SQRH

#include "hitable.h"

class sqr: public hitable{
	public:
		sqr(){}
		sqr(vec3 a,vec3 b,vec3 c,vec3 d, material *m) : A(a), B(b), C(c), D(d), mat_ptr(m) {};
		virtual bool hit (const ray& r, float tmin, 
							float tmax, hit_record& rec) const;
		vec3 A; 
		vec3 B; 
		vec3 C; 
		vec3 D; 
		material *mat_ptr;
};

bool sqr::hit (const ray& r, float t_min, float t_max, hit_record& rec) const{
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
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	n = cross(C-B,C-D);
	t = dot(n, (C - r.origin()))/dot(n, r.direction());
   	p = r.origin() + r.direction() * t;
	
	float val4 = dot(cross((C - A), (p - A)), n);
    	float val5 = dot(cross((A - D), (p - D)), n);
    	float val6 = dot(cross((D - C), (p - C)), n);

 	if(((val4 >= 0) && (val5 >= 0) && (val6 >= 0))||
      		((val4 < 0) && (val5 < 0) && (val6 < 0))){	
		if(t < t_max && t > t_min){
			rec.t = t;
			rec.p = p;
			rec.normal = n;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}

#endif
