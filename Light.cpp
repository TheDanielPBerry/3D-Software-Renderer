#include <vector>
#include <iostream>

#include "Plane.h"

typedef struct Light {
	Vec3f pos;
	Vec3f color;
	float luminosity;
} Light;

#define AMBIENT_LIGHT 0.03
void light_scene(std::vector<Plane> &scene, const std::vector<Light> lights)
{
	for(uint i=0; i<scene.size(); i++) {
		for(uint p=0; p<N_POINTS; p++) {
			//Vec3f lumos = Vec3f { AMBIENT_LIGHT, AMBIENT_LIGHT, AMBIENT_LIGHT };
			float lumos = AMBIENT_LIGHT;
			for(const Light &light : lights) {
				lumos += std::max((light.luminosity / distance_cubed(scene[i].points[p], light.pos)), (float)0);
	 			// lumos.x += intensity * light.color.x;
	 			// lumos.y += intensity * light.color.y;
	 			// lumos.z += intensity * light.color.z;
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
				0, -15, 5
			},
			.luminosity = 2000,
		}
	);

	for(int i = -20; i<0; i+=10) {
		lights.push_back(
			Light {
				.pos = Vec3f {
					(float)-i, 15, (float) i
				},
				.luminosity = 1000,
			}
		);
	}
}
