#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Type Definitions
// -----------------------------------------------------------------------------

typedef struct
{
    double x;
    double y;
    double z;
} Vec3;

// Aliases for code readability
typedef Vec3 Point3; // 3D point
typedef Vec3 Color;  // RGB Color

void vec3_construct(Vec3 *v, double x, double y, double z);

// Constructor
Vec3 vec3_create(double x, double y, double z);

// Vector + Vector
Vec3 vec3_add(Vec3 u, Vec3 v);

// Vector - Vector
Vec3 vec3_sub(Vec3 u, Vec3 v);

// Vector * Scalar
Vec3 vec3_scale(Vec3 v, double t);

// Vector * Vector (Element-wise / Hadamard product)
Vec3 vec3_mul(Vec3 u, Vec3 v);

// Vector / Scalar
Vec3 vec3_div(Vec3 v, double t);

// Dot Product: u . v
double vec3_dot(Vec3 u, Vec3 v);

// Cross Product: u x v
Vec3 vec3_cross(Vec3 u, Vec3 v);

// Length Squared (Faster)
double vec3_length_squared(Vec3 v);

// Length (Uses sqrt)
double vec3_length(Vec3 v);

// Unit Vector (Normalize)
Vec3 vec3_unit(Vec3 v);

// Reflect vector v around normal n
// Formula: v - 2*dot(v,n)*n
Vec3 vec3_reflect(Vec3 v, Vec3 n);

Vec3 vec3_refract(Vec3 uv, Vec3 n, double etai_over_etat);

Vec3 random_in_unit_sphere();

// Prints vector to console: "[x, y, z]"
void vec3_print(const Vec3 *v);

// Writes a Color to file in PPM format
void vec3_write_color(FILE *output, Color pixel_color);

#endif