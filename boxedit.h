#ifndef BOXEDIT_H
#define BOXEDIT_H

#include <vector>
#include <SDL2/SDL.h>

#include "game.h"
#include "Physics.h"
#include "Vec.h"


void draw_box(
	Box &box,
	const Vec3f &translate,
	Vec3f (&rotationTrig)[2],
	const Vec2f &dimensions,
	Uint32 *buffer,
	float *z_buffer
);

void set_box_of_interest(Box *box);
Box *get_box_of_interest();

void draw_bounding_boxes(
	std::vector<Box> &boxes,
	std::vector<Entity> &entities,
	const Vec3f &translate,
	const Vec3f &rotate,
	const Vec2f &dimensions,
	Uint32 *screen_buffer,
	float *z_buffer
);

void check_boxedit_keys(SDL_Event &event, Signals &signals);

void box_signals(Signals &signals, std::vector<Box> &staticBoxes, Entity *camera, Box *staticTree);

#endif
