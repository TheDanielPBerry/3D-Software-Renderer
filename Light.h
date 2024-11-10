#ifndef Light_h
#define Light_h

#include "Plane.h"
#include <vector>

typedef struct Light {
	Vec3f pos;
	Vec3f color;
	float luminosity;
} Light;

void light_scene(std::vector<Plane> &scene, const std::vector<Light> lights);

void some_lights(std::vector<Light> &lights);

#endif
