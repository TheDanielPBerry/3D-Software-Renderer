
#include <SDL2/SDL_scancode.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>


#include "Physics.h"
#include "boxedit.h"


void poll_controls(Entity *camera, Signals &signals)
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT: {
				signals.running = false; //close the program
				break;
			}

			case SDL_MOUSEMOTION: {
				#define MOUSE_SPEED 0.01
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
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_K: {
						signals.forward = false;
						break;
					}
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_H: {
						signals.left = false;
						break;
					}
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_J: {
						signals.back = false;
						break;
					}
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_L: {
						signals.right = false;
						break;
					}
					case SDL_SCANCODE_SPACE: {
						signals.jump = false;
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
						break;
					}
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_K: {
						signals.forward = true;
						break;
					}
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_H: {
						signals.left = true;
						break;
					}
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_J: {
						signals.back = true;
						break;
					}
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_L: {
						signals.right = true;
						break;
					}
					case SDL_SCANCODE_SPACE: {
						signals.jump = true;
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


