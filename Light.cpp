#include <vector>
#include <iostream>

#include "Plane.h"

typedef struct Light {
	Vec3f pos;
	Vec3f color;
	float luminosity;
} Light;

void light_scene(std::vector<Plane> &scene, const std::vector<Light> lights)
{
	for(uint i=0; i<scene.size(); i++) {
		for(uint p=0; p<N_POINTS; p++) {
			float lumos = 0.05;
			for(const Light &light : lights) {
	 			lumos += (light.luminosity / distance_squared(scene[i].points[p], light.pos));
			}
			scene[i].color[p].x = std::min(scene[i].color[p].x * lumos, scene[i].color[p].x);
			scene[i].color[p].y = std::min(scene[i].color[p].y * lumos, scene[i].color[p].y);
			scene[i].color[p].z = std::min(scene[i].color[p].z * lumos, scene[i].color[p].z);
		}
	}
}


void some_lights(std::vector<Light> &lights)
{
	lights.push_back(
		Light {
			.pos = Vec3f {
				0, -3, 5
			},
			.luminosity = 3,
		}
	);
}
