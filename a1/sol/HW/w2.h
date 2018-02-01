#pragma once

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "w2.h"

//nobady understand overload operator
//http://articles.emptycrate.com/2009/10/12/nobody_understands_c_part_8_operator_overloading.html

class vec3 {
	public:	
		vec3(){}
		vec3(float e0, float e1, float e2) {e[0] = e0; e[1] = e1; e[2] = e2;}
		inline float x() const { return e[0]; }
		inline float y() const { return e[1]; }
		inline float z() const { return e[2]; }
		inline float r() const { return e[0]; }
		inline float g() const { return e[1]; }
		inline float b() const { return e[2]; }
		
		inline const vec3& operator+() const {return *this;} 
		// const member function returning a const & 
		//Note: Assignment operations should always return a
		//reference to the new value 
		//(typically, the code would be something like: return *this;).
		
		inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
		// const member function 
		//Type operator-(const Type &rhs) const; // Subtraction
        //Type operator-() const; // Unary minu
		
		inline float operator[](int i) const { return e[i];}
		inline float& operator[](int i) { return e[i];}
		
		/*	
		  Type &operator&=(const Type &rhs); // Assign bitwise and
		  Type &operator^=(const Type &rhs); // Assign exclusive or
		  Type &operator|=(const Type &rhs); // Assign bitwise or
		  Type &operator-=(const Type &rhs); // Assign difference
		  Type &operator<<=(const Type &rhs); // Assign left shift
		  Type &operator*=(const Type &rhs); // Assign product
		  Type &operator/=(const Type &rhs); // Assign quotient
		  Type &operator%=(const Type &rhs); // Assign remainder
		  Type &operator>>=(const Type &rhs); // Assign right shift
		  Type &operator+=(const Type &rhs); // Assign sum
		*/

		inline vec3& operator+=(const vec3 &v2);//
		inline vec3& operator-=(const vec3 &v2);//	
		inline vec3& operator*=(const vec3 &v2);//
		inline vec3& operator/=(const vec3 &v2);//
		inline vec3& operator*=(const float t);//
		inline vec3& operator/=(const float t);//
		
		inline float length() const {//
			return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);	}
		inline float squared_length() const {//
			return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
		inline void make_unit_vector();//
		
		float e[3];

};

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
