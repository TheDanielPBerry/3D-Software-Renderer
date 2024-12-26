#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

#include "Physics.h"


#define N_POINTS 3

typedef struct Plane {
	Vec3f points[N_POINTS];
	Vec3f normals[N_POINTS];
	Vec3f normal;
	Vec3f buffer[N_POINTS];
	Vec4f color[N_POINTS];
	Vec2f texture_coords[N_POINTS];
	SDL_Surface *texture;
	bool cullable = false;
	Entity *entity;
} Plane;


#define FRUSTUM_VIEWPOINT_DISTANCE 0.001


bool transform(Plane &plane, const Vec3f &translate, const Vec3f &rotate)
{
	Vec3f cosine = Vec3f { cos(rotate.x), cos(rotate.y), 0};
	Vec3f sine = Vec3f { sin(rotate.x), sin(rotate.y), 0 };
	for(u_char p=0; p<N_POINTS; p++) {
		plane.buffer[p] = plane.points[p] + translate;
		float x = (plane.buffer[p].x * cosine.y) - (plane.buffer[p].z * sine.y);
		float z = (plane.buffer[p].z * cosine.y) + (plane.buffer[p].x * sine.y);
		
		float y = (plane.buffer[p].y * cosine.x) - (z * sine.x);
		z = (z * cosine.x) + (plane.buffer[p].y * sine.x);
		plane.buffer[p].x = x;
		plane.buffer[p].y = y;
		plane.buffer[p].z = z;
	}

	if(plane.cullable) {
		Vec3f camera = Vec3f{ -1, -1, -1} * plane.buffer[0];
		Vec3f normal = cross_product(plane.buffer[0] - plane.buffer[1], plane.buffer[0] - plane.buffer[2]);
		float dot = dot_product(normal, camera);
		return dot > 0;
	}
	return true;
}

/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_and_scale(Plane &plane, const Vec2f &dimensions)
{
	//const float ratio = (dimensions.x / dimensions.y) * 500;
	for(uint p=0; p<N_POINTS; p++) {
		float z = plane.buffer[p].z;
		if(z < FRUSTUM_VIEWPOINT_DISTANCE) {
			z = FRUSTUM_VIEWPOINT_DISTANCE;
		}

		plane.buffer[p].x = ((plane.buffer[p].x
			* (1 / (z + FRUSTUM_VIEWPOINT_DISTANCE))) + 0.5) * dimensions.x;
		
		plane.buffer[p].y = ((plane.buffer[p].y 
			* (1 / (z + FRUSTUM_VIEWPOINT_DISTANCE))) + 0.5) * dimensions.y;
	}
}
