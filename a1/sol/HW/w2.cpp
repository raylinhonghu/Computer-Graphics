#include <iostream>
#include "w2.h"

//There are, of course, notable and somewhat reasonable exceptions 
//to the canonical forms which are valid.
//For example, the “streaming” operator that the C++ standard library uses:

//			ostream &operator<<(ostream &os, const Type &out) {   
//  			return (os << out.to_string()); 
//			}

inline std::istream& operator>> (std::istream &is, vec3 & t){
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}


inline std::ostream& operator<< (std::ostream &os, const vec3 &t){
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void vec3::make_unit_vector() {
	float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline vec3 operator+ (const vec3 &v1, const vec3 &v2) {
	 return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator- (const vec3 &v1, const vec3 &v2) {
	 return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator* (const vec3 &v1, const vec3 &v2) {
	 return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/ (const vec3 &v1, const vec3 &v2) {
	 return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator* (float t, const vec3 &v) {
	 return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}	

inline vec3 operator* (const vec3 &v, float t) {
	 return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}	

inline float dot(const vec3 &v1, const vec3 &v2){
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2){
	return vec3((v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
				(-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
				(v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}

inline vec3 operator/ (const vec3 v, float t) {
	 return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}	

inline vec3& vec3::operator+=(const vec3 &v){
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v){
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v){
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v){
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const float t){
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t){
	float k = 1.0/t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline vec3 unit_vector(vec3 v){
	return v / v.length();
}

int main(){
	int nx = 200;
	int ny = 100;
	
	std::cout<< "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny-1; j >= 0; j --){
		for (int i = 0; i < nx; i++){
			vec3 col(float(i) / float(nx),float(j) / float(ny), 0.2);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " "<< ig << " " << ib << "\n";   
		}
	}
}
