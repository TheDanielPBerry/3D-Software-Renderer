#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#include "Physics.h"

typedef struct Sinals {
	bool running = true;
	bool forward = false;
	bool back = false;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool crouch = false;
} Signals;

void poll_controls(Entity *camera, Signals &signals);

void draw_bounding_boxes(
	std::vector<Box> &boxes,
	std::vector<Entity> &entities,
	const Vec3f &translate,
	const Vec3f &rotate,
	const Vec2f &dimensions,
	Uint32 *screen_buffer,
	float *z_buffer
);

#endif
