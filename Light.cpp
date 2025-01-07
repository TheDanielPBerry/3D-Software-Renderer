#include <vector>

#include "Plane.h"
#include "Vec.h"

typedef struct Light {
	Vec3f pos;
	Vec3f color = Vec3f{1.0, 1.0, 1.0};
	float constant;
	float linear;
	float quadratic;
} Light;


#define AMBIENT_LIGHT 0.4
void calculate_plane_vertex_lights(Plane &plane, std::vector<Light> &lights)
{
	for(uint p=0; p<N_POINTS; p++) {
		const uint one = (p+1) % N_POINTS;
		const uint two = (p+2) % N_POINTS;

		const Vec3f normal = normalize(cross_product(plane.buffer[p] - plane.buffer[one], plane.buffer[p] - plane.buffer[two]));
		Vec3f result = Vec3f{0,0,0};
		for(Light &light : lights) {
			float attenuation = light.constant;
			float distdist = distance_squared(light.pos, plane.points[p]); 
			attenuation += light.quadratic * distdist;
			float dist = sqrt(distdist);
			attenuation += light.linear * dist;
			attenuation = 1 / attenuation;
			//attenuation = 1.0;

			result = result + (light.color * AMBIENT_LIGHT * attenuation);
			//Diffuse calculation
			Vec3f lightDir = (light.pos - plane.buffer[p]) / dist;
			float diff = std::max(dot_product(normal, lightDir), (float) 0.0);

			result = result + (light.color * diff * attenuation);
		}
		plane.color[p].x = std::max((float)0, std::min(plane.color[p].x * result.x, (float)1.0));
		plane.color[p].y = std::max((float)0, std::min(plane.color[p].y * result.y, (float)1.0));
		plane.color[p].z = std::max((float)0, std::min(plane.color[p].z * result.z, (float)1.0));
	}
}


void light_scene(std::vector<Plane> &scene, std::vector<Light> &lights)
{
	for(uint i=0; i<scene.size(); i++) {
		scene[i].buffer[0] = scene[i].points[0];
		scene[i].buffer[1] = scene[i].points[1];
		scene[i].buffer[2] = scene[i].points[2];
		calculate_plane_vertex_lights(scene[i], lights);
	}
}


void some_lights(std::vector<Light> &lights)
{
	//Sun
	lights.push_back(
		Light {
			.pos = Vec3f {
				0.0,-600,480
			},
			.color = Vec3f{1.0, 1.0, 1.0},
			.constant = 0.000001,
			.linear = 0.00000000001,
			.quadratic = 0.000005,
		}
	);


	//Courtyard Lights
	lights.push_back(
		Light {
			.pos = Vec3f {
				-22.7341,-7.34089,-20.8009
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				-24.533,-9.79371,12.1226
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	//Library Lights
	lights.push_back(
		Light {
			.pos = Vec3f {
				10.347,-5.72349,-26.2754
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				-12.4549,-4.77474,-42.0326
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	//Station Lights
	lights.push_back(
		Light {
			.pos = Vec3f {
				-45.3701,-11.2328,-20.0964
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.32,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				-34.3701,-11.2328,-20.1464
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.32,
		}
	);

	//West side throne torches
	lights.push_back(
		Light {
			.pos = Vec3f {
				4.9,-7.2,-7.0
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				4.9,-7.2,-11.0
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				4.9,-7.2,-15.0
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);

	//East side throne torches
	lights.push_back(
		Light {
			.pos = Vec3f {
				-5.0,-7.2,-6.48849	
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				-5.0,-7.2,-10.48849	
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
	lights.push_back(
		Light {
			.pos = Vec3f {
				-5.0,-7.2,-14.48849	
			},
			.color = Vec3f{1.0, 0.6, 0.0},
			.constant = 0.05,
			.linear = 0.4,
			.quadratic = 0.032,
		}
	);
}
