#include <iostream>
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "box.h"
#include "tri.h"
#include "sqr.h"
#include "smoke.h"
#include "surface_texture.h"
#include "aarect.h"
#include "constant_medium.h"
#include "texture.h"
#include "cornell_ball.h"
#include "cornell_box.h"
#include "rgba.h"
#include "color.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

hitable *red() {
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("red.jpg", &nx, &ny, &nn, 0);
    material *mat =  new lambertian(new image_texture(tex_data, nx, ny));
    return new sphere(vec3(0,0, 0), 2, mat);
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    rgba8* pixels = new rgba8[nx * ny];

    memset(pixels, 0, nx * ny * sizeof(*pixels));

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *world = cornell_balls();

    vec3 lookfrom(478, 278, -600);
    vec3 lookat(278,278,0);

    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;

    camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);


    for (int j = ny-1; j >= 0; j-- ) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0,0,0);

            for(int s = 0; s < ns; s++){
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u,v);
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
    stbi_write_png("raytrace.png", nx, ny, 4, pixels, nx * 4);

#ifdef _WIN32
    system("raytrace.png");
#else
    system("open raytrace.png");
#endif

    delete[] pixels;

}

