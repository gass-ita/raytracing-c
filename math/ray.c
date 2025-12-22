#include "ray.h"

Ray ray_create(Point3 origin, Vec3 direction)
{
    Ray r = {origin, direction};
    return r;
}

// return the point along the ray at distance t
// P(t) = A + t*B
Point3 ray_at(Ray r, double t)
{
    Vec3 scaled_direction = vec3_scale(r.direction, t);
    return vec3_add(r.origin, scaled_direction);
}
