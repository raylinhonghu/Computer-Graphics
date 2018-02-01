
#include <iostream>
#include "hitableish.h"
#include "sphere.h"
#include "hitable.h"
#include "float.h"
#include "camera.h"

#include <stdlib.h>

vec3 random_in_unit_sphere(){
	vec3 p;
	do{
		p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r, hitable *world){
	hit_record rec;
	if(world->hit(r, 0.0, FLT_MAX, rec)){
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5* color(ray(rec.p, target-rec.p), world);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}
