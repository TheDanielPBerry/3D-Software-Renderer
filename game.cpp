
#include <SDL2/SDL_scancode.h>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>


#include "Physics.h"
#include "lines.h"

typedef struct Sinals {
	bool running = true;
	bool forward = false;
	bool back = false;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool crouch = false;
} Signals;

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
				#define MOUSE_SPEED 0.03
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
				if(event.key.keysym.scancode == 5) {
					std::cout << "Debugger? I hardly know her.";
				} 
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
						std::cout << event.key.keysym.scancode << std::endl;
						break;
					}
				}
				break;
			}
		}
	}
}


void draw_bounding_boxes(
	std::vector<Box> &boxes,
	std::vector<Entity> &entities,
	const Vec3f &translate,
	const Vec3f &rotate,
	const Vec2f &dimensions,
	Uint32 *screen_buffer,
	float *z_buffer
)
{
	Vec3f rotationTrig[2] = {
		Vec3f{cos(rotate.x), cos(rotate.y), 1},
		Vec3f{sin(rotate.x), sin(rotate.y), 0},
	};

	for(Box &box : boxes) {
		draw_box(box, translate, rotationTrig, dimensions, screen_buffer, z_buffer);
	}
	for(Entity &entity : entities) {
		for(Box &box : entity.boxes) {
			draw_box(box, translate, rotationTrig, dimensions, screen_buffer, z_buffer);
		}
	}
}
