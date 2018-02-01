// MiniMFB: for creating a window and rendering to it
#include "MiniFB.h"

// GLM: for matrix/vector math
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_precision.hpp"

#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace glm;

// gamma correction
u8vec4 hdr_to_ldr(vec3 hdr)
{
    return u8vec4((u8vec3)(255.0f * pow(hdr, vec3(1.0f / 2.2f))), 255);
}

// "0xAARRGGBB" on a little endian system = "BB GG RR AA" in memory
u8vec4 to_bgra(u8vec4 rgba)
{
    return u8vec4(rgba.b, rgba.g, rgba.r, rgba.a);
}

// https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
mat4 make_inf_reversed_z_proj(float fovy, float aspect, float zNear)
{
    float f = 1.0f / tan(fovy / 2.0f);
    return mat4(
        f / aspect, 0.0f,  0.0f,  0.0f,
              0.0f,    f,  0.0f,  0.0f,
              0.0f, 0.0f,  0.0f, -1.0f,
              0.0f, 0.0f, zNear,  0.0f);
}

void rasterize_triangle(
    const vec4 clip_verts_in[3],  // vertex position in clip space
    const vec3 colors_in[3], // vertex color (RGB)
    int fbwidth, int fbheight, u8vec4* framebuffer, float* depthbuffer)
{
    vec4 clip_verts[3] = {
        clip_verts_in[0], clip_verts_in[1], clip_verts_in[2]
    };

    vec3 colors[3] = {
        colors_in[0], colors_in[1], colors_in[2],
    };

    // far plane clipping
    // (note: actually implements near plane clipping. reversed z magic!)
    {
        bvec3 is_clipped;
        is_clipped = bvec3(clip_verts[0].z >= clip_verts[0].w, clip_verts[1].z >= clip_verts[1].w, clip_verts[2].z >= clip_verts[2].w);

        int num_clipped = is_clipped.x + is_clipped.y + is_clipped.z;

        if (num_clipped == 3)
        {
            // clip whole triangle with 3 vertices behind clip plane
            return;
        }

        if (num_clipped == 2)
        {
            // Two vertices behind the clipping plane. In this case, cut the associated edges short.

            int unclipped = 0;
            if (!is_clipped[1]) unclipped = 1;
            if (!is_clipped[2]) unclipped = 2;

            int v1 = (unclipped + 1) % 3;
            int v2 = (unclipped + 2) % 3;

            // clip intersection parameters
            float a1 = (clip_verts[unclipped].z - clip_verts[unclipped].w) / ((clip_verts[unclipped].z - clip_verts[unclipped].w) - (clip_verts[v1].z - clip_verts[v1].w));
            float a2 = (clip_verts[unclipped].z - clip_verts[unclipped].w) / ((clip_verts[unclipped].z - clip_verts[unclipped].w) - (clip_verts[v2].z - clip_verts[v2].w));

            // clip the first edge
            clip_verts[v1] = (1.0f - a1) * clip_verts[unclipped] + a1 * clip_verts[v1];
            clip_verts[v1].z = clip_verts[v1].w;

            // clip the second edge
            clip_verts[v2] = (1.0f - a2) * clip_verts[unclipped] + a2 * clip_verts[v2];
            clip_verts[v2].z = clip_verts[v1].w;

            colors[v1] = (1.0f - a1) * colors[unclipped] + a1 * colors[v1];
            colors[v2] = (1.0f - a2) * colors[unclipped] + a2 * colors[v2];
        }

        if (num_clipped == 1)
        {
            // One vertex behind the clipping plane. In this case, triangulate the triangle into two triangles.

            int clipped = 0;
            if (is_clipped[1]) clipped = 1;
            if (is_clipped[2]) clipped = 2;

            int v1 = (clipped + 1) % 3;
            int v2 = (clipped + 2) % 3;

            // clip intersection parameters
            float a1 = (clip_verts[clipped].z - clip_verts[clipped].w) / ((clip_verts[clipped].z - clip_verts[clipped].w) - (clip_verts[v1].z - clip_verts[v1].w));
            float a2 = (clip_verts[clipped].z - clip_verts[clipped].w) / ((clip_verts[clipped].z - clip_verts[clipped].w) - (clip_verts[v2].z - clip_verts[v2].w));

            // clip the first edge
            vec4 clipped_vert1;
            clipped_vert1 = (1.0f - a1) * clip_verts[clipped] + a1 * clip_verts[v1];
            clipped_vert1.z = 0.0f;

            // clip the second edge
            vec4 clipped_vert2;
            clipped_vert2 = (1.0f - a2) * clip_verts[clipped] + a2 * clip_verts[v2];
            clipped_vert2.z = clipped_vert2.w;

            vec3 clipped_color1 = (1.0f - a1) * colors[clipped] + a1 * colors[v1];
            vec3 clipped_color2 = (1.0f - a2) * colors[clipped] + a2 * colors[v2];

            // output the first clipped triangle (note: recursive call)
            vec4 clipped_verts[3] = { clip_verts[0], clip_verts[1], clip_verts[2] };
            vec3 clipped_colors[3] = { colors[0], colors[1], colors[2] };
            clipped_verts[clipped] = clipped_vert1;
            clipped_colors[clipped] = clipped_color1;

            rasterize_triangle(clipped_verts, clipped_colors, fbwidth, fbheight, framebuffer, depthbuffer);

            // set self up to output the second clipped triangle
            clip_verts[clipped] = clipped_vert2;
            clip_verts[v1] = clipped_vert1;
            colors[clipped] = clipped_color2;
            colors[v1] = clipped_vert1;
        }
    }

    vec3 one_over_w = vec3(1.0f / clip_verts[0].w, 1.0f / clip_verts[1].w, 1.0f / clip_verts[2].w);

    // clip space to window coordinates
    i64vec2 window_xy[3];
    float window_z[3];
    for (int i = 0; i < 3; i++)
    {
        // scale to window coordinates and convert to fixed point
        window_xy[i].x = (int64_t)((+clip_verts[i].x * one_over_w[i] + 1.0f) / 2.0f * (int64_t)fbwidth * 256);
        window_xy[i].y = (int64_t)((-clip_verts[i].y * one_over_w[i] + 1.0f) / 2.0f * (int64_t)fbheight * 256);
        window_z[i] = clip_verts[i].z * one_over_w[i];
    }

    float min_depth = min(min(window_z[0], window_z[1]), window_z[2]);
    float max_depth = max(max(window_z[0], window_z[1]), window_z[2]);

    // triangle bounding box
    i64vec2 window_bbox_min = min(min(window_xy[0], window_xy[1]), window_xy[2]) >> i64vec2(8);
    i64vec2 window_bbox_max = max(max(window_xy[0], window_xy[1]), window_xy[2]) >> i64vec2(8);

    if (window_bbox_max.x < 0 ||
        window_bbox_max.y < 0 ||
        window_bbox_min.x >= fbwidth ||
        window_bbox_min.y >= fbheight)
    {
        // triangle is fully outside the window
        return;
    }

    // clamp bounding box to window
    window_bbox_min = max(window_bbox_min, i64vec2(0));
    window_bbox_max = min(window_bbox_max, i64vec2(fbwidth - 1, fbheight - 1));

    int64_t triarea2_64 = ((window_xy[2].x - window_xy[0].x) * (window_xy[1].y - window_xy[0].y) - (window_xy[2].y - window_xy[0].y) * (window_xy[1].x - window_xy[0].x));

    if (triarea2_64 == 0)
    {
        // empty triangle
        return;
    }

    if (triarea2_64 < 0)
    {
        // backface culling
        return;
    }

    // precompute 1/triarea2
    float one_over_triarea2 = 1.0f / triarea2_64;

    // compute barycentric coordinate at first pixel, and the derivatives of the barycentrics
    i64vec3 bary;
    i64vec3 dbarydx, dbarydy;
    for (int v = 0; v < 3; v++)
    {
        int v1 = (v + 1) % 3;
        int v2 = (v + 2) % 3;

        dbarydx[v] = window_xy[v2].y - window_xy[v].y;
        dbarydy[v] = window_xy[v].x  - window_xy[v2].x;

        // 0x80 = subpixel offset of first sample (0.5 pixels)
        const int64_t s168_zero_pt_five = 0x80;
        bary[v] = (s168_zero_pt_five - window_xy[v].x) * dbarydx[v] - ((s168_zero_pt_five - window_xy[v].y) * -dbarydy[v]);
        
        // give dbarys 16 fractional bits (from 8) to match the fractional bits of "bary"
        dbarydx[v] <<= 8;
        dbarydy[v] <<= 8;

        // Top-left rule: shift top-left edges ever so slightly outward to make the top-left edges be the tie-breakers when rasterizing adjacent triangles
        if ((window_xy[v].y == window_xy[v1].y && window_xy[v].x < window_xy[v1].x) || window_xy[v].y > window_xy[v1].y)
        {
            bary[v]--;
        }
    }

    // offset bary to the start of the triangle's bounding box
    bary += dbarydx * window_bbox_min.x;
    bary += dbarydy * window_bbox_min.y;

    // loop through triangle's bounding box and compute triangle coverage
    for (int64_t y = window_bbox_min.y; y <= window_bbox_max.y; y++)
    {
        i64vec3 row_bary = bary;

        for (int64_t x = window_bbox_min.x; x <= window_bbox_max.x; x++)
        {
            if (all(lessThan(row_bary, i64vec3(0))))
            {
                // barycentric coordinates of the 2D (projected) triangle
                vec3 bary2d = vec3(-row_bary) * one_over_triarea2;

                float depth = dot(bary2d, vec3(window_z[1], window_z[2], window_z[0]));
                depth = clamp(depth, min_depth, max_depth);

                // note: invertez Z test for increased precision
                if (depth > depthbuffer[y * fbwidth + x])
                {
                    // barycentric coordinates of the 3D triangle (from perspective corrected 2D triangle) 
                    vec3 f = bary2d * one_over_w;
                    vec3 bary3d;
                    bary3d.x = f.x / (f.x + f.y + f.z);
                    bary3d.y = f.y / (f.x + f.y + f.z);
                    bary3d.z = 1.0f - bary3d.x - bary3d.y;

                    vec3 color = vec3(
                        dot(bary3d, vec3(colors[1].r, colors[2].r, colors[0].r)),
                        dot(bary3d, vec3(colors[1].g, colors[2].g, colors[0].g)),
                        dot(bary3d, vec3(colors[1].b, colors[2].b, colors[0].b)));

                    framebuffer[y * fbwidth + x] = to_bgra(hdr_to_ldr(color));
                    depthbuffer[y * fbwidth + x] = depth;
                }
            }

            row_bary += dbarydx;
        }

        bary += dbarydy;
    }
}

void render_cube(
    mat4 modelviewproj,
    int fbwidth, int fbheight, u8vec4* framebuffer, float* depthbuffer)
{
    const int num_tris = 12;
    vec3 cube_vertices[num_tris * 3] = {
        // left
        vec3(-1,-1,-1), vec3(-1,+1,+1), vec3(-1,+1,-1),
        vec3(-1,+1,+1), vec3(-1,-1,-1), vec3(-1,-1,+1),

        // right
        vec3(+1,-1,+1), vec3(+1,-1,-1), vec3(+1,+1,+1),
        vec3(+1,+1,+1), vec3(+1,-1,-1), vec3(+1,+1,-1),

        // top
        vec3(+1,+1,-1), vec3(-1,+1,-1), vec3(-1,+1,+1),
        vec3(+1,+1,-1), vec3(-1,+1,+1), vec3(+1,+1,+1),

        // bottom
        vec3(-1,-1,-1), vec3(+1,-1,-1), vec3(-1,-1,+1),
        vec3(-1,-1,+1), vec3(+1,-1,-1), vec3(+1,-1,+1),
        
        // front
        vec3(-1,-1,+1), vec3(+1,-1,+1), vec3(+1,+1,+1),
        vec3(-1,-1,+1), vec3(+1,+1,+1), vec3(-1,+1,+1),
        
        // back
        vec3(-1,-1,-1), vec3(-1,+1,-1), vec3(+1,-1,-1),
        vec3(+1,-1,-1), vec3(-1,+1,-1), vec3(+1,+1,-1)
    };

    vec3 grass = vec3(0.183f, 0.448f, 0.058f);
    vec3 dirt = vec3(0.302f, 0.148f, 0.0677f);

    vec3 cube_colors[num_tris / 2] = {
        // left
        dirt,

        // right
        dirt,

        // top
        grass,

        // bottom
        dirt,
        
        // front
        dirt,
        
        // back
        dirt
    };

    for (int tri = 0; tri < num_tris; tri++)
    {
        vec4 verts[] = {
            modelviewproj * vec4(cube_vertices[tri * 3 + 0], 1.0f),
            modelviewproj * vec4(cube_vertices[tri * 3 + 1], 1.0f),
            modelviewproj * vec4(cube_vertices[tri * 3 + 2], 1.0f)
        };

        vec3 colors[] = {
            cube_colors[tri / 2],
            cube_colors[tri / 2],
            cube_colors[tri / 2]
        };

        rasterize_triangle(
            verts, colors,
            fbwidth, fbheight, framebuffer, depthbuffer);
    }
}

void render_scene(
    float curr_time,
    int fbwidth, int fbheight, u8vec4* framebuffer, float* depthbuffer)
{
    // clear framebuffer
    for (int i = 0; i < fbwidth * fbheight; i++)
    {
        framebuffer[i] = to_bgra(u8vec4(100, 149, 237, 255)); // cornflower blue
    }

    // clear depthbuffer
    for (int i = 0; i < fbwidth * fbheight; i++)
    {
        // note: clear to 0 because of reversed Z
        depthbuffer[i] = 0.0f;
    }

    // camera setup
    vec3 eye = vec3(0.0f, 7.0f, 7.0f);
    vec3 target = vec3(0, 0, 0);
    vec3 up = vec3(0, 1, 0);
    mat4 view = lookAt(eye, target, up);
    mat4 proj = make_inf_reversed_z_proj(radians(70.0f), (float)fbwidth / fbheight, 0.01f);

    ////////////////////
    // TODO: Rotate the whole scene together by setting this matrix.
    ////////////////////
    mat4 scene = mat4()* rotate(1.0f*curr_time,vec3(0.0f,1.0f,0.0f));

    // rotated cube
    {
        ////////////////////
        // TODO: Rotate this cube by rotating the model matrix.

        ////////////////////
        mat4 model = mat4();
        mat4 modelviewproj = proj * view * scene * model * translate(vec3(5.0f,2.0f,2.0f)) * rotate(1.0f*curr_time,vec3(0.0f,1.0f,0.0f))  * scale(vec3(0.5f,0.5f,0.5f));

        render_cube(modelviewproj, fbwidth, fbheight, framebuffer, depthbuffer);
    }

    // translated cube
    {
        ////////////////////

        // TODO: Translate this cube by translating the model matrix.
        ////////////////////
        mat4 model = mat4();

        mat4 modelviewproj = proj * view * scene * model * translate(vec3(3.0f,2.0f,2.0f));

        render_cube(modelviewproj, fbwidth, fbheight, framebuffer, depthbuffer);
    }

    // scaled cube
    {
        ////////////////////
        // TODO: Scale this cube by scaling the model matrix.
        ////////////////////
        mat4 model = mat4();

        mat4 modelviewproj = proj * view * scene * model * scale(vec3(2.0f,2.0f,2.0f));

        render_cube(modelviewproj, fbwidth, fbheight, framebuffer, depthbuffer);
    }
}

int main()
{
    // dimensions of the window and framebuffer
    int width = 640;
    int height = 480;

    // allocate framebuffer for rendering
    u8vec4* framebuffer = new u8vec4[width * height];
    float* depthbuffer = new float[width * height];

    // open the window
    if (!mfb_open("transforms", width, height))
    {
        fprintf(stderr, "Failed to open window\n");
        return 1;
    }

    clock_t start = clock();

    // main loop
    for (;;)
    {
        clock_t now = clock();
        float curr_time = (float)(now - start) / CLOCKS_PER_SEC;

        // render the scene into framebuffer
        render_scene(curr_time, width, height, framebuffer, depthbuffer);

        // update the window with the contents of framebuffer
        if (mfb_update(framebuffer) < 0)
        {
            break;
        }
    }

    // close the window
    mfb_close();

    delete[] framebuffer;
    delete[] depthbuffer;
}
