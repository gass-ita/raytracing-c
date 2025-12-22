#include "scene.h"

#include "math/ray.h"
#include "math/vec3.h"
#include "hittable.h"

#include <stdlib.h>

Camera camera_create(Point3 center, Point3 lower_left_corner, Vec3 horizontal, Vec3 vertical, int image_width, int image_height)
{
    Camera cam;
    cam.center = center;
    cam.pixel_delta_u = vec3_div(horizontal, (double)image_width);
    cam.pixel_delta_v = vec3_div(vertical, (double)image_height);
    cam.pixel00_loc = lower_left_corner;
    cam.image_width = image_width;
    cam.image_height = image_height;
    return cam;
}

// return a vector with random components in [-0.5 , 0.5]
static Vec3
random_vec()
{
    return vec3_create((double)rand() / RAND_MAX - 0.5, (double)rand() / RAND_MAX - 0.5, (double)rand() / RAND_MAX - 0.5);
}

Color ray_color(Scene *scene, Ray r, int depth)
{
    HitRecord rec;
    bool hit_anything = false;
    double closest_so_far = 1000.0; // Infinity-ish
    double t_min = 0.0001;          // Minimum distance (prevents shadow acne - colliding with self)

    if (depth <= 0)
        return vec3_create(0, 0, 0);

    for (int i = 0; i < scene->hittable_count; i++)
    {
        HitRecord temp_rec;
        if (hit_hittable(&scene->world[i], r, t_min, closest_so_far, &temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    if (hit_anything)
    {
        Vec3 unit_direction = vec3_unit(r.direction);
        Vec3 reflected = vec3_reflect(unit_direction, rec.normal);
        Ray scattered = ray_create(rec.p, reflected);

        // Call the function on the reflected ray
        Color reflection_color = ray_color(scene, scattered, depth - 1);

        // Interpolate the base color of the object with the reflection color based on reflectivity
        double r = rec.material.reflectivity;

        Color matte_part = vec3_scale(rec.material.color, 1.0 - r);
        Color shiny_part = vec3_scale(reflection_color, r);

        return vec3_add(matte_part, shiny_part);
    }

    // --- 3. Background (Sky) ---
    Vec3 unit_direction = vec3_unit(r.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    Color white = vec3_create(1.0, 1.0, 1.0);
    Color blue = vec3_create(0.5, 0.7, 1.0);
    return vec3_add(vec3_scale(white, 1.0 - t), vec3_scale(blue, t));
}

// Returns a vector with random x and y in [-0.5, 0.5], z is 0.
static Vec3 sample_square()
{
    return vec3_create(
        (double)rand() / RAND_MAX - 0.5,
        (double)rand() / RAND_MAX - 0.5,
        0.0);
}

Ray camera_get_ray(Camera *cam, int pixel_x, int pixel_y)
{

    Vec3 pixel_center = cam->pixel00_loc;

    pixel_center = vec3_add(pixel_center, vec3_scale(cam->pixel_delta_u, pixel_x));
    pixel_center = vec3_add(pixel_center, vec3_scale(cam->pixel_delta_v, pixel_y));

    Vec3 offset = sample_square();
    Vec3 pixel_sample = pixel_center;
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_u, offset.x));
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_v, offset.y));

    Vec3 ray_origin = cam->center;
    Vec3 ray_direction = vec3_sub(pixel_sample, ray_origin);

    return ray_create(ray_origin, ray_direction);
}

void render_scene(FILE *output, Scene *scene, Camera *camera, int max_depth)
{

    fprintf(output, "P3\n%d %d\n255\n", camera->image_width, camera->image_height);
    for (int j = camera->image_height - 1; j >= 0; j--)
    {
        for (int i = 0; i < camera->image_width; i++)
        {
            Ray r = camera_get_ray(camera, i, j);
            Color pixel_color = ray_color(scene, r, max_depth);
            vec3_write_color(output, pixel_color);
        }
    }
}
