#ifndef SCENE_H
#define SCENE_H

#include "math/vec3.h"
#include "math/ray.h"
#include <stdbool.h> // for bool, true, false

// -----------------------------------------------------------------------------
// Hittable objects
// -----------------------------------------------------------------------------
typedef enum
{
    HITTABLE_SPHERE,
    HITTABLE_PLANE,
    HITTABLE_TRIANGLE
} HittableType;

typedef enum
{
    MATERIAL_LAMBERTIAN,
    MATERIAL_METAL,
    MATERIAL_DIELECTRIC
} MaterialType;

typedef struct
{
    Point3 point;
    Vec3 normal;
} Plane;

typedef struct
{
    Color color;

    MaterialType type;
    union
    {
        double fuzz;    // For metal materials
        double ref_idx; // For dielectric materials
    } properties;

} Material;

typedef struct
{
    Point3 v0;
    Point3 v1;
    Point3 v2;
} Triangle;

typedef struct
{
    Point3 center;
    double radius;
} Sphere;

typedef struct t_hittable
{
    HittableType type; // Type of the hittable object
    union
    {
        Sphere sphere;
        Triangle triangle;
        Plane plane;
    } object;          // The actual object data
    Material material; // Material properties of the hittable object
} Hittable;

// -----------------------------------------------------------------------------
// Hit Record: Stores data about where a ray hit an object
// -----------------------------------------------------------------------------
typedef struct
{
    double t;          // Distance along the ray
    Point3 p;          // The exact point of intersection
    Vec3 normal;       // The surface normal at that point
    Material material; // Material of the hit object
    bool front_face;   // Whether the hit was on the outside surface
} HitRecord;

// returns true if the ray hits the sphere between t_min and t_max, recording the information in rec
bool hit_sphere(const Sphere *s, Material material, Ray r, double t_min, double t_max, HitRecord *rec);

bool hit_plane(const Plane *p, Material material, Ray r, double t_min, double t_max, HitRecord *rec);

bool hit_triangle(const Triangle *tr, Material material, Ray r, double t_min, double t_max, HitRecord *rec);

bool hit_hittable(const Hittable *h, Ray r, double t_min, double t_max, HitRecord *rec);

bool scatter_ray(const Material *material, Ray r_in, HitRecord *rec, Color *attenuation, Ray *scattered);

#endif // SCENE_H