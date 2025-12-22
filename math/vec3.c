#include "vec3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void vec3_construct(Vec3 *v, double x, double y, double z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

Vec3 vec3_create(double x, double y, double z)
{
    Vec3 v = {};
    vec3_construct(&v, x, y, z);
    return v;
}

Vec3 vec3_add(Vec3 u, Vec3 v)
{
    return vec3_create(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vec3 vec3_sub(Vec3 u, Vec3 v)
{
    return vec3_create(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vec3 vec3_scale(Vec3 v, double t)
{
    return vec3_create(v.x * t, v.y * t, v.z * t);
}

Vec3 vec3_mul(Vec3 u, Vec3 v)
{
    return vec3_create(u.x * v.x, u.y * v.y, u.z * v.z);
}

Vec3 vec3_div(Vec3 v, double t)
{
    return vec3_scale(v, 1.0 / t);
}

double vec3_dot(Vec3 u, Vec3 v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vec3 vec3_cross(Vec3 u, Vec3 v)
{
    return vec3_create(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x);
}

double vec3_length_squared(Vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double vec3_length(Vec3 v)
{
    return sqrt(vec3_length_squared(v));
}

Vec3 vec3_unit(Vec3 v)
{
    return vec3_div(v, vec3_length(v));
}

Vec3 vec3_reflect(Vec3 v, Vec3 n)
{
    return vec3_sub(v, vec3_scale(n, 2 * vec3_dot(v, n)));
}

Vec3 vec3_refract(Vec3 uv, Vec3 n, double etai_over_etat)
{
    double cos_theta = fmin(vec3_dot(vec3_scale(uv, -1.0), n), 1.0);
    Vec3 r_out_perp = vec3_scale(vec3_add(uv, vec3_scale(n, cos_theta)), etai_over_etat);
    Vec3 r_out_parallel = vec3_scale(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}
// Returns a random real in [0,1)
double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min,max)
double random_double_range(double min, double max)
{
    return min + (max - min) * random_double();
}

Vec3 random_in_unit_sphere()
{
    Vec3 p;
    do
    {
        // Generate a vector where x, y, z are between -1 and 1
        p = vec3_create(
            random_double_range(-1, 1),
            random_double_range(-1, 1),
            random_double_range(-1, 1));
    } while (vec3_length_squared(p) >= 1.0); // If outside sphere, try again
    return p;
}

void vec3_print(const Vec3 *v)
{
    fprintf(stderr, "[%f, %f, %f]\n", v->x, v->y, v->z);
}

void vec3_write_color(FILE *output, Color pixel_color)
{
    // Write the translated [0,255] value of each color component.
    // We assume pixel_color values are between 0.0 and 1.0

    // Optional: Gamma correction (gamma 2.0)
    // r = sqrt(r), etc.
    // You can add that here later.

    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    // Clamp values to [0.0, 0.999] before casting to int
    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);

    fprintf(output, "%d %d %d\n", ir, ig, ib);
}