#ifndef Plane_h
#define Plane_h

#include <SDL2/SDL.h>

#include "Physics.h"


#define N_POINTS 3

typedef struct Plane {
	Vec3f points[N_POINTS];
	Vec3f normals[N_POINTS];
	Vec3f normal;
	Vec3f buffer[N_POINTS];
	Vec4f color[N_POINTS];
	Vec2f texture_coords[N_POINTS];
	SDL_Surface *texture = nullptr;
	bool cullable = false;
	bool cameraStatic = false;
	Entity *entity;
} Plane;



bool transform(Plane &plane, const Vec3f &translate, const Vec3f rotationTrig[2]);

uint clip_plane(Plane plane, Plane (&planes)[2]);
/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_and_scale(Plane &plane, const Vec2f &dimensions);

#endif
