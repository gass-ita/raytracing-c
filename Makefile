all: raytracing.exe
	raytracing.exe > output.ppm

raytracing.exe: .\main.c .\math\vec3.c .\math\ray.c hittable.c scene.c
	gcc .\main.c .\math\vec3.c .\math\ray.c hittable.c scene.c  -o raytracing.exe -lm