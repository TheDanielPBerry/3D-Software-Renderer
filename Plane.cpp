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
	bool cameraStatic = false;
	Entity *entity;
} Plane;


#define FRUSTUM_VIEWPOINT_DISTANCE 0.01


bool transform(Plane &plane, const Vec3f &translate, const Vec3f rotationTrig[2])
{
	#define COSINE 0
	#define SINE 1

	for(u_char p=0; p<N_POINTS; p++) {
		plane.buffer[p] = plane.points[p];
		if(plane.cameraStatic) {
			continue;
		}
		float x, y, z, temp_y;
		if(plane.entity != nullptr) {
			rotate(plane.buffer[p], plane.entity->rotationMatrix);
		}
		plane.buffer[p] = plane.buffer[p] + translate;
		if(plane.entity != nullptr) {
			plane.buffer[p] = plane.buffer[p] + plane.entity->pos;
		}

		x = (plane.buffer[p].x * rotationTrig[COSINE].y) - (plane.buffer[p].z * rotationTrig[SINE].y);
		z = (plane.buffer[p].z * rotationTrig[COSINE].y) + (plane.buffer[p].x * rotationTrig[SINE].y);
		
		y = (plane.buffer[p].y * rotationTrig[COSINE].x) - (z * rotationTrig[SINE].x);
		z = (z * rotationTrig[COSINE].x) + (plane.buffer[p].y * rotationTrig[SINE].x);

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

uint clip_plane(Plane plane, Plane (&planes)[2])
{
	uint planes_to_draw = 1;
	uint behind_z = 0;
	uint in_front_z = 0;
	u_char behind[3];
	u_char in_front[3];

	for(u_char p=0; p<N_POINTS; p++) {
		if(plane.buffer[p].z < FRUSTUM_VIEWPOINT_DISTANCE) {
			behind[behind_z] = p;
			behind_z++;
		} else {
			in_front[in_front_z] = p;
			in_front_z++;
		}
	}
	if(behind_z == 0) {
		planes[0] = plane;
		return 1;
	}
	else if(behind_z == 1) {
		planes[0] = plane;
		planes[1] = plane;

		Vec2f coefficients;

		Vec2f texture_coords[2];
		Vec3f buffers[2];
		Vec4f colors[2];
		for(u_char p=0; p<in_front_z; p++) {
			coeffs(Vec2f{ plane.buffer[behind[0]].x, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.buffer[in_front[p]].x, plane.buffer[in_front[p]].z }, coefficients);
			buffers[p].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.buffer[behind[0]].y, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.buffer[in_front[p]].y, plane.buffer[in_front[p]].z }, coefficients);
			buffers[p].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			buffers[p].z = FRUSTUM_VIEWPOINT_DISTANCE;

			//Colors
			coeffs(Vec2f{ plane.color[behind[0]].x, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.color[in_front[p]].x, plane.buffer[in_front[p]].z }, coefficients);
			colors[p].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.color[behind[0]].y, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.color[in_front[p]].y, plane.buffer[in_front[p]].z }, coefficients);
			colors[p].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.color[behind[0]].z, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.color[in_front[p]].z, plane.buffer[in_front[p]].z }, coefficients);
			colors[p].z = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			//Textures
			coeffs(Vec2f{ plane.texture_coords[behind[0]].x, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.texture_coords[in_front[p]].x, plane.buffer[in_front[p]].z }, coefficients);
			texture_coords[p].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.texture_coords[behind[0]].y, plane.buffer[behind[0]].z }, 
				Vec2f{ plane.texture_coords[in_front[p]].y, plane.buffer[in_front[p]].z }, coefficients);
			texture_coords[p].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);
		}
		planes[0].buffer[0] = plane.buffer[in_front[0]];
		planes[0].buffer[1] = buffers[0];
		planes[0].buffer[2] = buffers[1];

		planes[0].color[0] = plane.color[in_front[0]];
		planes[0].color[1] = colors[0];
		planes[0].color[2] = colors[1];

		planes[0].texture_coords[0] = plane.texture_coords[in_front[0]];
		planes[0].texture_coords[1] = texture_coords[0];
		planes[0].texture_coords[2] = texture_coords[1];


		planes[1].buffer[0] = plane.buffer[in_front[0]];
		planes[1].buffer[1] = plane.buffer[in_front[1]];
		planes[1].buffer[2] = buffers[1];
		
		planes[1].color[0] = plane.color[in_front[0]];
		planes[1].color[1] = plane.color[in_front[1]];
		planes[1].color[2] = colors[1];

		planes[1].texture_coords[0] = plane.texture_coords[in_front[0]];
		planes[1].texture_coords[1] = plane.texture_coords[in_front[1]];
		planes[1].texture_coords[2] = texture_coords[1];

		return 2;
	}
	else if(behind_z == 2) {
		planes[0] = plane;

		Vec2f coefficients;
		for(u_char p=0; p<behind_z; p++) {
			coeffs(Vec2f{ plane.buffer[behind[p]].x, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.buffer[in_front[0]].x, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].buffer[behind[p]].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.buffer[behind[p]].y, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.buffer[in_front[0]].y, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].buffer[behind[p]].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			planes[0].buffer[behind[p]].z = FRUSTUM_VIEWPOINT_DISTANCE;

			//Colors
			coeffs(Vec2f{ plane.color[behind[p]].x, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.color[in_front[0]].x, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].color[behind[p]].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.color[behind[p]].y, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.color[in_front[0]].y, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].color[behind[p]].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.color[behind[p]].z, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.color[in_front[0]].z, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].color[behind[p]].z = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);


			coeffs(Vec2f{ plane.texture_coords[behind[p]].x, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.texture_coords[in_front[0]].x, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].texture_coords[behind[p]].x = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);

			coeffs(Vec2f{ plane.texture_coords[behind[p]].y, plane.buffer[behind[p]].z }, 
				Vec2f{ plane.texture_coords[in_front[0]].y, plane.buffer[in_front[0]].z }, coefficients);
			planes[0].texture_coords[behind[p]].y = line(coefficients, FRUSTUM_VIEWPOINT_DISTANCE);
		}
		
		return 1;
	}
	return 0;
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
		plane.buffer[p].x = ((plane.buffer[p].x
			* (1 / (z))) + 0.5) * dimensions.x;
		
		plane.buffer[p].y = ((plane.buffer[p].y 
			* (1 / (z))) + 0.5) * dimensions.y;
	}
}
