
#include "game.h"
#include <SDL2/SDL_scancode.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>


#include "Physics.h"
#include "boxedit.h"


void poll_controls(Entity *camera, Signals &signals)
{
	unsigned short signal;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT: {
				signals.running = false; //close the program
				break;
			}

			case SDL_MOUSEMOTION: {
				#define MOUSE_SPEED 0.1
				camera->rotational_velocity.y += MOUSE_SPEED * event.motion.xrel;
				const float targetVelocity = MOUSE_SPEED * event.motion.yrel;
				const float targetRotation = camera->rotation.x + targetVelocity;

				#define PI_OVER_TWO 1.41	//Aproxmiation
				if(event.motion.yrel > 0 && targetRotation < PI_OVER_TWO) {
					camera->rotational_velocity.x += targetVelocity;
				} else if(event.motion.yrel < 0 && targetRotation > -PI_OVER_TWO) {
					camera->rotational_velocity.x += targetVelocity;
				}
				break;
			}

			case SDL_KEYUP: {
				switch(event.key.keysym.scancode) {
					case SDL_SCANCODE_W: {
						signals.forward = false;
						signal &= ~SIGNAL_FORWARD;
						break;
					}
					case SDL_SCANCODE_A: {
						signals.left = false;
						signal &= ~SIGNAL_LEFT;
						break;
					}
					case SDL_SCANCODE_S: {
						signals.back = false;
						signal &= ~SIGNAL_BACK;
						break;
					}
					case SDL_SCANCODE_D: {
						signals.right = false;
						signal &= ~SIGNAL_RIGHT;
						break;
					}
					case SDL_SCANCODE_SPACE: {
						signals.jump = false;
						signal &= ~SIGNAL_JUMP;
						break;
					}
					case SDL_SCANCODE_LSHIFT: {
						signals.crouch = false;
						break;
					}
					default: {
						break;
					}
				}
				break;
			}

			case SDL_KEYDOWN: {
				switch(event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE: {
						signals.running = false;
						signal |= SIGNAL_QUIT;
						break;
					}
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_K: {
						signals.forward = true;
						signal |= SIGNAL_FORWARD;
						break;
					}
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_H: {
						signals.left = true;
						signal |= SIGNAL_LEFT;
						break;
					}
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_J: {
						signals.back = true;
						signal |= SIGNAL_BACK;
						break;
					}
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_L: {
						signals.right = true;
						signal |= SIGNAL_RIGHT;
						break;
					}
					case SDL_SCANCODE_SPACE: {
						signals.jump = true;
						signal |= SIGNAL_JUMP;
						break;
					}
					case SDL_SCANCODE_LSHIFT: {
						signals.crouch = true;
						break;
					}
					default: {
						check_boxedit_keys(event, signals);
						std::cout << event.key.keysym.scancode << std::endl;
						break;
					}
				}
				break;
			}
		}
	}
}


