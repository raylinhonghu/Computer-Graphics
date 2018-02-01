/*
	#ifndef x                 //先测试x是否被宏定义过
	#define x
	   程序段1 :blabla~    //如果x没有被宏定义过，定义x，并编译程序段 1
	#endif   
	　 　程序段2 :blabla~　　 //如果x已经定义过了则编译程序段2的语句，“忽视”程序段 1
*/


#ifndef RAYH
#define RAYH
#include "w2.h"

class ray
{
	public: 
		ray(){}
		ray(const vec3& a, const vec3& b) { A = a; B = b; }
		vec3 origin() const   { return A; } // no change but read OK
		vec3 direction() const  { return B; } // no change but read OK
		vec3 point_at_parameter (float t) const { return A + t * B; } // no change but read OK

		vec3 A;
		vec3 B;
};

#endif
