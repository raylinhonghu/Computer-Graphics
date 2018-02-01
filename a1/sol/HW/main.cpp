#include <iostream>
#include "hitableish.h"
#include "sphere.h"
#include "hitable.h"
#include "float.h"
#include "camera.h"
#include "tri.h"
#include "sqr.h"
#include "material.h"
#include <stdlib.h>
#include "plane.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>
#include "texture.h"
//#include "aabb.h"
//#include "aarect.h"
//#include "box.h"
//#include "bvh.h"


struct rgba8 {
    unsigned char r, g, b, a;
    rgba8() { }
    rgba8(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
    : r(rr),g(gg),b(bb),a(aa) { }
};

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) { 
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
             return attenuation*color(scattered, world, depth+1);
        }
        else {
           return emitted;
        }
    }
    else {
       // vec3 unit_direction = unit_vector(r.direction());
       // float t = 0.5*(unit_direction.y() + 1.0);
        //return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        return vec3(0,0,0);
    }
}

/*
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0,-100.5, -1), 100,new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
*/

int main (){
/*
    int width = 640;
    int height = 480;
    */


    int nx = 200;
    int ny = 100;
	int ns = 100;
    rgba8* pixels = new rgba8[nx * ny];

    memset(pixels, 0, nx * ny * sizeof(*pixels));

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	/*
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	*/
/*
	list[0] = new sphere(vec3(0,0,-1), 0.3, new lambertian(vec3(0.8,0.3,0.3)));		//sphere
	//list[1] = new plane(vec3(-4,2,-4),vec3(-4,-2,-4),vec3(-4,-2,-8),vec3(-4,2,-8),new lambertian(vec3(0.2,0.2,0.2))); //left
	//list[2] = new plane(vec3(-4,2,-4),vec3(4,2,-4),vec3(4,2,-8),vec3(-4,2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//top
	//list[3] = new plane(vec3(4,2,-4),vec3(4,-2,-4),vec3(4,-2,-8),vec3(4,2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//right
	list[1] = new plane(vec3(0.5,0.3,-0.5),vec3(0.5,-0.3,-0.5),vec3(0.5,0.3,-1.5),vec3(0.5,-0.3,-1.5),new lambertian(vec3(0.8,0.8,0)));
	//list[4] = new plane(vec3(-4,-2,-4),vec3(4,-2,-4),vec3(4,-2,-8),vec3(-4,-2,-8),new lambertian(vec3(0.2,0.2,0.2)));   //bot
	//list[1] = new plane(vec3(-0.5,-0.3,-0.5),vec3(0.5,-0.3,-0.5),vec3(0.5,-0.3,-1.5),vec3(-0.5,-0.3,-1.5),new lambertian(vec3(0.8,0.8,0)));   //bot
	list[2] = new plane(vec3(-0.5,0.3,-0.5),vec3(0.5,0.3,-0.5),vec3(0.5,0.3,-1.5),vec3(-0.5,0.3,-1.5),new lambertian(vec3(0.8,0.8,0)));   //top
	//list[5] = new plane(vec3(4,-2,-8),vec3(4,2,-8),vec3(-4,2,-8),vec3(-4,-2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//inside
	//list[1] = new sphere(vec3(0,-100.3,-1), 100, new lambertian(vec3(0.8,0.3,0.3)));
////
//	list[6] = new sphere(vec3(0,0,-1), 0.5);	
//	list[1] = new sqr(vec3(-1,-0.5,3),vec3(1,-0.5,3),vec3(1,-0.5,-2),vec3(-1,-0.5,-2));   //bot
	//list[3] = new sphere(vec3(-1,-0.3,-1), 0.3, new metal(vec3(0.5, 0.5, 0.5), 0.0));
	//list[2] = new sphere(vec3(1,-0.3,-0.7), 0.2, new metal(vec3(0.3, 0.3, 0.3), 0.0));
	//list[1] = new sphere(vec3(0,-100.5,-1), 100, new metal(vec3(0.8, 0.6, 0.2), 0.0));


*/   hitable *list[11];
    texture *mos = new noise_texture(4);
    texture *checker = new checker_texture (new constant_texture(vec3(0.2,0.3,0.1)),new constant_texture (vec3(0.6,0.6,0.6)));
    list[0] = new sphere(vec3(-0.4f, 0.0f,-1.0f), 0.3f, new metal(vec3(1.0, 1.0, 1.0), 0.0)); //gold : 0.8,0.6,0.2
    list[1] = new sphere(vec3(0.5f,-0.1f,-1.1f), 0.4f, new dielectric(1.5));
    //list[0] = new sphere(vec3(0,0,-1), 0.3, new lambertian(vec3(0.8,0.3,0.3)));		//sphere
    list[2] = new sqr(vec3(-2.0f,-0.5f,-2.5f), vec3( 2.0f,-0.5f,-2.5f), vec3( 2.0f,-0.5f,-0.5f), vec3(-2.0f,-0.5f,-0.5f), new lambertian(checker));//bot
    //list[2] = new plane(vec3(-4,-2,-4),vec3(4,-2,-4),vec3(4,-2,-8),vec3(-4,-2,-8),new lambertian(vec3(0.2,0.2,0.2)));   //bot
    list[3] = new sqr(vec3(-2.0f,-0.5f,-2.5f), vec3(-2.0f,-0.5f,-0.5f), vec3(-2.0f, 2.0f,-0.5f), vec3(-2.0f, 2.0f,-2.5f), new lambertian(new constant_texture(vec3(1.0f, 0.3f, 0.3f))));//left
    //list[3] = new plane(vec3(-4,-2,-4),vec3(-4,-2,-4),vec3(-4,2,-8),vec3(-4,2,-8),new lambertian(vec3(0.2,0.2,0.2))); //left
    list[4] = new sqr(vec3( 2.0f,-0.5f,-0.5f), vec3( 2.0f,-0.5f,-2.5f), vec3( 2.0f, 2.0f,-2.5f), vec3( 2.0f, 2.0f,-0.5f), new lambertian(new constant_texture(vec3(0.1f, 0.2f, 0.5f))));//right
    //list[4] = new plane(vec3(4,-2,-4),vec3(4,-2,-4),vec3(4,2,-8),vec3(4,2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//right
    list[5] = new sqr(vec3(-2.0f, 2.0f,-2.5f), vec3(-2.0f, 2.0f,-0.5f), vec3( 2.0f, 2.0f,-0.5f), vec3( 2.0f, 2.0f,-2.5f), new lambertian(new constant_texture(vec3(0.1f, 0.1f, 0.1f))));//top
    //list[5] = new plane(vec3(-4,2,-4),vec3(-4,2,-4),vec3(4,2,-8),vec3(4,2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//top
    list[6] = new sqr(vec3(-2.0f,-0.5f,-2.5f), vec3(-2.0f, 2.0f,-2.5f), vec3( 2.0f, 2.0f, -2.5f), vec3( 2.0f,-0.5f,-2.5f), new lambertian(new constant_texture(vec3(0.5f, 0.5f, 0.5f))));//inside
    //list[6] = new plane(vec3(-4,-2,-8),vec3(-4,2,-8),vec3(4,2,-8),vec3(4,-2,-8),new lambertian(vec3(0.2,0.2,0.2)));	//inside
    list[7] = new sphere(vec3(-0.0f,0.5f,-1.2f),0.35f,new lambertian(mos));
    list[8] = new xz_rect(0,550,0,550,555,new lambertian(new constant_texture(vec3(0.73,0.73,0.73))));
     list[10] = new xz_rect(0,551,0,551,0,new lambertian(new constant_texture(vec3(0.73,0.73,0.73))));
    //list[10] = new xz_rect(-2,2,-0.5,-2.5,0,new diffuse_light(new constant_texture(vec3(4,4,4))));
   // list[11] = new xz_rect(0,555,0,555,0,new diffuse_light(new constant_texture(vec3(4,4,4))));
 //   list[8] = new xz_rect(0,555,0,555,555,new diffuse_light(new constant_texture(vec3(4,4,4))));
  //  list[9] = new xz_rect(0,200,0,200,0,new diffuse_light(new constant_texture(vec3(4,4,4))));
list[9] = new xz_rect(0,555,0,555,250,new diffuse_light(new constant_texture(vec3(4,4,4))));
    /*
 * list[]
    list[2] = new sqr(vec3(-8.0f,-2.0f,-8.0f), vec3( 8.0f,-2.0f,-8.0f), vec3( 8.0f,-2.0f,-2.0f), vec3(-8.0f,-2.0f,-2.0f), new lambertian(vec3(0.5f, 0.5f, 0.5f)));//bot
    list[3] = new sqr(vec3(-8.0f,-2.0f,-8.0f), vec3(-8.0f,-2.0f,-2.0f), vec3(-8.0f, 8.0f,-2.0f), vec3(-8.0f, 8.0f,-8.0f), new lambertian(vec3(1.0f, 0.3f, 0.3f)));//left
    list[4] = new sqr(vec3( 8.0f,-2.0f,-2.0f), vec3( 8.0f,-2.0f,-8.0f), vec3( 8.0f, 8.0f,-8.0f), vec3( 8.0f, 8.0f,-2.0f), new lambertian(vec3(0.1f, 0.2f, 0.5f)));//right
    list[5] = new sqr(vec3(-8.0f, 8.0f,-8.0f), vec3(-8.0f, 8.0f,-2.0f), vec3( 8.0f, 8.0f,-2.0f), vec3( 8.0f, 8.0f,-8.0f), new lambertian(vec3(0.1f, 0.1f, 0.1f)));//top
    list[6] = new sqr(vec3(-8.0f,-2.0f,-8.0f), vec3(-8.0f, 8.0f,-8.0f), vec3( 8.0f, 8.0f, -8.0f), vec3(8.0f,-2.0f,-8.0f), new lambertian(vec3(0.5f, 0.5f, 0.5f)));//inside
*/
    hitable *world = new hitable_list(list,11);

    camera cam;

/*

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

*/

    #pragma omp parallel for
    for (int j = ny-1; j >= 0; j-- ) {
            for (int i = 0; i < nx; i++) {
                vec3 col(0,0,0);

				for(int s = 0; s < ns; s++){
					float u = float(i + drand48()) / float(nx);
					float v = float(j + drand48()) / float(ny); 


                  //  vec3 bary;

					ray r = cam.get_ray(u,v);
                    //vec3 p = r.point_at_parameter(2.0);
					col += color(r, world,0);

				}
				col /= float(ns);
                col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));
				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);
			
                pixels[i+(ny-j-1)*nx] =  rgba8((unsigned char)ir, (unsigned char)ig, (unsigned char)ib, 255);
			
			}
	}

    // write image to file
    stbi_write_png("raytrace.png", nx, ny, 4, pixels, nx * 4);

#ifdef _WIN32
    // Display the image automatically on Windows
    system("raytrace.png");
#else
    // Display the image automatically on MacOS
    system("open raytrace.png");
#endif

    delete[] pixels;

}
