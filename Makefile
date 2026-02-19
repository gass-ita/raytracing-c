all: raytracing
	./raytracing > output.ppm

raytracing: main.c math/vec3.c math/ray.c hittable.c scene.c
	gcc main.c math/vec3.c math/ray.c hittable.c scene.c  -o raytracing -lm
