#pragma once

#include <iostream>
#include "sphere.h"

#include "material.h"
#include "surface_texture.h"
#include "constant_medium.h"
#include "texture.h"


hitable *cornell_balls() {
    hitable **list = new hitable*[9];
    int i = 0;
    material *red = new lambertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(vec3(5, 5, 5)) );
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    hitable *boundary = new sphere(vec3(160, 100, 145), 100, new dielectric(1.5));
    list[i++] = boundary;
    list[i++] = new constant_medium(boundary, 0.1, new constant_texture(vec3(1.0, 1.0, 1.0)));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white),  15), vec3(265,0,295));
    return new hitable_list(list,i);
}
