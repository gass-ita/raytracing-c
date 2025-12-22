#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct
{
    Point3 origin;
    Vec3 direction;
} Ray;

Ray ray_create(Point3 origin, Vec3 direction);

// Returns the point at parameter t along the ray.
// Formula: P(t) = origin + (direction * t)
Point3 ray_at(Ray r, double t);

#endif // RAY_H