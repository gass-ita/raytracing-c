#pragma once

#include "math/vec3.h"
#include "math/ray.h"
#include "hittable.h"

#define MAX_HITTABLES 100

// -----------------------------------------------------------------------------
typedef struct
{
    int image_width;
    int image_height;
    Point3 center;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Point3 pixel00_loc;
    size_t samples_per_pixel;
} Camera;

typedef struct
{
    Hittable world[MAX_HITTABLES];
    size_t hittable_count;
} Scene;

Camera camera_create(Point3 center, Point3 lower_left_corner, Vec3 horizontal, Vec3 vertical, int image_width, int image_height, size_t samples_per_pixel);
Color ray_color(Scene *scene, Ray r, int depth);
Ray camera_get_ray(Camera *cam, int pixel_x, int pixel_y);
void render_scene(FILE *output, Scene *scene, Camera *camera, int max_depth);