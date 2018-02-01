#include <iostream>
#include "ray.h"
//t*tdot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - RR = 0
// C = center point 
//
float hit_sphere (const vec3& center, float radius, const ray& r){
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction()); //dot(B,B)
	float b = 2.0 * dot(oc, r.direction());      //2*t*dot(A-C,B)
	float c = dot(oc, oc) - radius * radius;     //dot(A-C,A-C) - RR
	float discriminant  = b*b - 4.0*a*c;
	if (discriminant < 0) {
		return -1.0;
	}else{
		return (-b -sqrt(discriminant)) / (2.0*a);
	}
	//return (discriminant < 0);
}

/*
ax^{2}+bx+c,is
Here for real a, b and c,
 if Δ > 0, the polynomial has two real roots, 
 if Δ = 0, the polynomial has one real double root, 
 and if Δ < 0, the two roots of the polynomial are complex conjugates.
*/

vec3 color(const ray& r){
	
	float t = hit_sphere(vec3(0,0,-1), 0.5, r); 
	if(t > 0.0){
		vec3 N = unit_vector(r.point_at_parameter(t) -vec3(0, 0, -1));
		return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
	}
	//if (hit_sphere(vec3(0,0,-1),0.5,r))
	//	return vec3(1,0,0);
	//	return vec3(0,0,0);//change sphere color
	//						//111white 100red 00black
	vec3 unit_direction = unit_vector(r.direction()); //faded
	t = 0.5*(unit_direction.y() + 1.0);//faded
	return (1.0 - t)*vec3(1.0,1.0,1.0) + t*vec3(0.5, 0.7, 1.0);//faded
}

int main (){
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	
	for (int j = ny-1; j >= 0; j-- ) {
			for (int i = 0; i < nx; i++) {
				float u = float(i) / float(nx); // (u,v)where the point 
				float v = float(j) / float(ny); // is on the plane
				ray r(origin,lower_left_corner + u * horizontal + v * vertical);
				vec3 col = color(r); //blend color
				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);
			
				std::cout << ir << " " << ig << " " << ib << "\n";
			
			}
	}
}
