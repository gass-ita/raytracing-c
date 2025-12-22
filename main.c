#include <stdio.h>
#include <stdbool.h>
#include "math/vec3.h"
#include "math/ray.h"
#include "hittable.h" // Include our new struct definitions
#include <time.h>
#include "scene.h"

#define WIDTH 1920
#define HEIGHT 1080

Hittable world[] = {
    {HITTABLE_SPHERE, .object.sphere = {{0, 0, -1}, 0.5}, {.color = {0.1, 0.2, 0.5}, .type = MATERIAL_LAMBERTIAN}}, // Center sphere
    {HITTABLE_SPHERE, .object.sphere = {{1, 0, -1.5}, 0.5}, {
                                                                .color = {0.3, 0.3, 0.7},
                                                                .type = MATERIAL_DIELECTRIC,
                                                                .properties.ref_idx = 0.9,
                                                            }}, // Right sphere
    {HITTABLE_SPHERE, .object.sphere = {{-1, 0, -1.5}, 0.5}, {
                                                                 .color = {0.3, 0.7, 0.3},
                                                                 .type = MATERIAL_METAL,
                                                                 .properties.fuzz = 0.0,
                                                             }}, // Left sphere
    {HITTABLE_PLANE, .object.plane = {{0, -0.5, 0}, {0, 1, 0}}, {
                                                                    .color = {0.8, 0.6, 0.2},
                                                                    .type = MATERIAL_LAMBERTIAN,
                                                                }}, // Ground Plane,
};

#define NUM_HITTABLES (sizeof(world) / sizeof(world[0]))

int main()
{
    Scene scene = {0};
    scene.hittable_count = NUM_HITTABLES;
    for (size_t i = 0; i < NUM_HITTABLES; i++)
    {
        scene.world[i] = world[i];
    }

    Camera camera = camera_create(
        vec3_create(0, 0.0, 0.5),
        vec3_create(-2.0, -1.725, -0.5),
        vec3_create(4.0, 0, 0),
        vec3_create(0, 2.25, 0),
        WIDTH / 3,
        HEIGHT / 3, 100); // 100 samples per pixel

    render_scene(stdout, &scene, &camera, 10);
    return 0;
}