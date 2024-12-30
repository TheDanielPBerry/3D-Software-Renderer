#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#include "Physics.h"

typedef struct Signals {
	bool running = true;
	bool forward = false;
	bool back = false;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool crouch = false;

	bool addBox = false;
	bool deleteBox = false;
	bool nextBox = false;
	bool prevBox = false;
} Signals;

void poll_controls(Entity *camera, Signals &signals);


#endif
