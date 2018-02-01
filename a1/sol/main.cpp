#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct rgba8 {
    unsigned char r, g, b, a;
    rgba8() { }
    rgba8(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
    : r(rr),g(gg),b(bb),a(aa) { }
};
struct vec3 {
    float x, y, z;
    vec3() { }
    vec3(float xx, float yy, float zz)
    : x(xx), y(yy), z(zz) { }
};
struct ray {
    vec3 o, d;
    ray() { }
    ray(vec3 oo, vec3 dd)
    : o(oo), d(dd) { }
};
struct triangle {
    vec3 a, b, c;
    triangle() { }
    triangle(vec3 aa, vec3 bb, vec3 cc)
    : a(aa), b(bb), c(cc) { }
};

rgba8 hdr_to_ldr(vec3 hdr) { return rgba8((unsigned char)(255 * powf(hdr.x, 1.0f / 2.2f)), (unsigned char)(255 * powf(hdr.y, 1.0f / 2.2f)), (unsigned char)(255 * powf(hdr.z, 1.0f / 2.2f)), 255); }
vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y); }
float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float length(vec3 v) { return sqrtf(dot(v, v)); }
vec3 operator+(vec3 a, vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
vec3 operator-(vec3 a, vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
vec3 operator-(vec3 v) { return vec3(-v.x, -v.y, -v.z); }
vec3 operator*(float u, vec3 v) { return vec3(u * v.x, u * v.y, u * v.z); }
vec3 operator*(vec3 v, float u) { return u * v; }
vec3 operator/(vec3 v, float u) { return vec3(v.x / u, v.y / u, v.z / u); }

bool intersect(ray r, triangle tr, vec3* bary)
{
    // TODO: Compute normal of triangle (see slide 3 of ray3.pdf.)
    vec3 n = cross((tr.b- tr.a), (tr.b - tr.c));

    // TODO: Compute distance of ray origin to triangle (see slide 3 of ray3.pdf.)
    float t = dot(n, (tr.b - r.o))/dot(n, r.d);

    // TODO: Compute point of intersection (see slide 3 of ray3.pdf.)
    vec3 p = r.o + r.d * t;

    // TODO: Compute the 3 equations in the green square on the right side of slide 3 of ray3.pdf.
    //       If the signs of the three equations differ, then return false.
    float val1 = dot(cross((tr.b - tr.a), (p - tr.a)), n);
    float val2 = dot(cross((tr.c - tr.b), (p - tr.b)), n);
    float val3 = dot(cross((tr.a - tr.c), (p - tr.c)), n);

    if(((val1 >= 0) && (val2 >= 0) && (val3 >= 0))||
        ((val1 < 0) && (val2 < 0) && (val3 < 0))){

    }else { return false;}
    // TODO: Compute barycentric coordinates at point p.
    // See slide 12 of ray3.pdf.
    // Hint: area(A,B,C) == length(cross(B - A, C - A)) / 2
    float alpha = (length(cross(tr.b - p, tr.c - p)) / 2) / (length(cross( tr.b - tr.a,  tr.c - tr.a)) / 2);
    float beta = (length(cross(tr.c - p, tr.a - p)) / 2) / (length(cross(tr.b - tr.a, tr.c - tr.a )) / 2);
    float garmma = (length(cross( tr.a - p, tr.b - p)) / 2) / (length(cross(tr.b - tr.a, tr.c - tr.a)) / 2);
    bary->y = alpha;
    bary->x = beta;
    bary->z = garmma;
    return true;
}

void raytrace()
{
    // allocate image
    int width = 640;
    int height = 480;
    rgba8* pixels = new rgba8[width * height];

    // clear to black
    memset(pixels, 0, width * height * sizeof(*pixels));

    // the triangle to intersect
    triangle tr = triangle(vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // construct ray for this pixel (orthographic projection)
            ray r;
            r.o = vec3(x / (width / 2.0f) - 1.0f, (height - y - 1) / (height / 2.0f) - 1.0f, 0.0f);
            r.d = vec3(0.0f, 0.0f, 1.0f);

            // test ray intersection with the triangle
            vec3 bary;
            if (intersect(r, tr, &bary))
            {
                // if the triangle intersected, output the barycentric coordinate as color.
                pixels[y * width + x] = hdr_to_ldr(bary);
            }
        }
    }

    // write image to file
    stbi_write_png("raytrace.png", width, height, 4, pixels, width * 4);

#ifdef _WIN32
    // Display the image automatically on Windows
    system("raytrace.png");
#else
    // Display the image automatically on MacOS
    system("open raytrace.png");
#endif

    delete[] pixels;
}

void rasterize()
{
    // allocate image
    int width = 640;
    int height = 480;
    rgba8* pixels = new rgba8[width * height];

    // clear to black
    memset(pixels, 0, width * height * sizeof(*pixels));

    // the triangle to intersect
    // note: z = 0 since we assume it's already projected onto the screen...
    triangle tr = triangle(vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f));

    // TODO: Compute double the area of the triangle.
    float double_area = length(cross((tr.b - tr.a), (tr.c - tr.a)));

    // top left of the image (initial evaluation for the barycentric)
    vec3 p = vec3(-1.0f, 1.0f, 0.0f);

    // TODO: compute barycentric coordinates at p. (x = -1, y = 1, the top left corner of image)
    vec3 bary;
    bary.x = -(length(cross(tr.b - p, tr.c - p)) / 2) / (length(cross(tr.b - tr.a, tr.c - tr.a)) / 2);
    bary.y = -(length(cross(tr.c - p, tr.a - p)) / 2) / (length(cross(tr.b - tr.a, tr.c - tr.a)) / 2);
    bary.z = -(length(cross(tr.a - p, tr.b - p)) / 2) / (length(cross(tr.b - tr.a, tr.c - tr.a)) / 2);

    // TODO: Compute screen-space derivatives of barycentric coordinates
    vec3 dbarydx;
    vec3 dbarydy;

    dbarydx.x = (tr.b - tr.a).y;
    dbarydx.y = (tr.c - tr.b).y;
    dbarydx.z = (tr.a - tr.c).y;

    dbarydy.x = -(tr.b - tr.a).x;
    dbarydy.y = -(tr.c - tr.b).x;
    dbarydy.z = -(tr.a - tr.c).x;

    dbarydx = dbarydx/double_area;
    dbarydy = dbarydy/double_area;

    // scale the coordinate space of the derivatives to fit the image size
    dbarydx = dbarydx / (width / 2.0f);
    dbarydy = dbarydy / (height / 2.0f);

    for (int y = 0; y < height; y++)
    {
        vec3 row_bary = bary;

        for (int x = 0; x < width; x++)
        {
            // test if all barycentrics are inside the triangle
            if (row_bary.x < 0.0f && row_bary.y < 0.0f && row_bary.z < 0.0f)
            {
                // if the triangle intersected, output the barycentric coordinate as color.
                pixels[y * width + x] = hdr_to_ldr(-row_bary);
            }

            row_bary = row_bary + dbarydx;
        }

        bary = bary - dbarydy;
    }

    // write image to file
    stbi_write_png("rasterize.png", width, height, 4, pixels, width * 4);

#ifdef _WIN32
    // Display the image automatically on Windows
    system("rasterize.png");
#else
    // Display the image automatically on MacOS
    system("open rasterize.png");
#endif

    delete[] pixels;
}

int main()
{
    // comment out one of these
 //   raytrace();
     rasterize();
}
