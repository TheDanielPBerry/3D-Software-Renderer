#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

#include "Physics.h"

#define SIGNAL_QUIT		0b10000000
#define SIGNAL_FORWARD	0b01000000
#define SIGNAL_BACK		0b00100000
#define SIGNAL_LEFT		0b00010000
#define SIGNAL_RIGHT	0b00001000
#define SIGNAL_JUMP		0b00000100
#define SIGNAL_CROUCH	0b00000010
#define SIGNAL_SHOOT	0b00000001


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
