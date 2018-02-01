#pragma once

#include <iostream>
#include "sphere.h"

#include "material.h"
#include "surface_texture.h"
#include "constant_medium.h"
#include "texture.h"


hitable *cornell_box() {
    hitable **list = new hitable*[8];
    int i = 0;
    material *red = new lambertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(vec3(15, 15, 15)) );
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130,0,65));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white),  15), vec3(265,0,295));
    return new hitable_list(list,i);
}
