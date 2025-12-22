#include "hittable.h"
#include <math.h>

// Implementation moved from hittable.h

bool hit_sphere(const Sphere *s, Material material, Ray r, double t_min, double t_max, HitRecord *rec)
{
    Vec3 oc = vec3_sub(r.origin, s->center);
    double a = vec3_length_squared(r.direction);
    double half_b = vec3_dot(oc, r.direction);
    double c = vec3_length_squared(oc) - s->radius * s->radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
    {
        return false;
    }

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
        {
            return false;
        }
    }

    // Success! We hit the sphere. Record the data.
    rec->t = root;
    rec->p = ray_at(r, root);

    // Calculate Normal: (Point - Center) / Radius
    Vec3 outward_normal = vec3_div(vec3_sub(rec->p, s->center), s->radius);

    // check for inword or outward normal
    if (vec3_dot(r.direction, outward_normal) > 0)
        rec->normal = vec3_scale(outward_normal, -1.0); // Inward
    else
        rec->normal = outward_normal;

    rec->material = material;

    return true;
}

bool hit_plane(const Plane *p, Material material, Ray r, double t_min, double t_max, HitRecord *rec)
{
    double denominator = vec3_dot(p->normal, r.direction);

    // If denominator is near zero, ray is parallel to plane -> No hit
    if (fabs(denominator) < 1e-6)
        return false;

    Vec3 p0l0 = vec3_sub(p->point, r.origin);
    double t = vec3_dot(p0l0, p->normal) / denominator;

    if (t < t_min || t > t_max)
        return false;

    rec->t = t;
    rec->p = ray_at(r, t);
    rec->material = material;

    // Face forward check (make plane double-sided)
    if (denominator > 0)
        rec->normal = vec3_scale(p->normal, -1.0);
    else
        rec->normal = p->normal;

    return true;
}

bool hit_triangle(const Triangle *tr, Material material, Ray r, double t_min, double t_max, HitRecord *rec)
{
    Vec3 v0v1 = vec3_sub(tr->v1, tr->v0);
    Vec3 v0v2 = vec3_sub(tr->v2, tr->v0);
    Vec3 pvec = vec3_cross(r.direction, v0v2);
    double det = vec3_dot(v0v1, pvec);

    if (fabs(det) < 1e-8)
        return false; // Parallel

    double inv_det = 1.0 / det;
    Vec3 tvec = vec3_sub(r.origin, tr->v0);
    double u = vec3_dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return false;

    Vec3 qvec = vec3_cross(tvec, v0v1);
    double v = vec3_dot(r.direction, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = vec3_dot(v0v2, qvec) * inv_det;
    if (t < t_min || t > t_max)
        return false;

    rec->t = t;
    rec->p = ray_at(r, t);
    rec->material = material;

    // Calculate normal from cross product
    Vec3 outward_normal = vec3_unit(vec3_cross(v0v1, v0v2));

    // Face forward check
    if (vec3_dot(r.direction, outward_normal) > 0)
        rec->normal = vec3_scale(outward_normal, -1.0);
    else
        rec->normal = outward_normal;

    return true;
}

bool hit_hittable(const Hittable *h, Ray r, double t_min, double t_max, HitRecord *rec)
{
    switch (h->type)
    {
    case HITTABLE_SPHERE:
        return hit_sphere(&(h->object.sphere), h->material, r, t_min, t_max, rec);
    case HITTABLE_PLANE:
        return hit_plane(&(h->object.plane), h->material, r, t_min, t_max, rec);
    case HITTABLE_TRIANGLE:
        return hit_triangle(&(h->object.triangle), h->material, r, t_min, t_max, rec);
    default:
        return false;
    }
}
