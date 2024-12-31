#ifndef Light_h
#define Light_h

#include "Plane.h"
#include <vector>

typedef struct Light {
	Vec3f pos;
	Vec3f color = Vec3f{1.0, 1.0, 1.0};
	float constant;
	float linear;
	float quadratic;
} Light;

void calculate_plane_vertex_lights(Plane &plane, std::vector<Light> &lights);
void light_scene(std::vector<Plane> &scene, std::vector<Light> &lights);

void some_lights(std::vector<Light> &lights);

#endif
