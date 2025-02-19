#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_hints.h>
#include <iostream>
#include <ctime>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

#include "rasterize.h"
#include "Physics.h"
#include "boxedit.h"
#include "boxedit.h"
#include "player.h"


long long getCurrentMilliseconds() {
	using namespace std::chrono;
	auto now = system_clock::now();                 // Get current time as system clock
	auto duration = now.time_since_epoch();         // Get duration since epoch
	return duration_cast<milliseconds>(duration).count(); // Convert to milliseconds
}

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


int init_game(std::vector<Plane> &scene, Entity *camera, Box *staticTree, std::vector<Entity> &entities, std::vector<Light> &lights, std::vector<Box> &staticBoxes)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Rect screen_rect = SDL_Rect{0, 0, int(DM.w*1), int(DM.h*1)};

	Vec2f dimensions = Vec2f{ (float)floor(screen_rect.w/2.0), (float)floor(screen_rect.h/2.0) };

	SDL_Window* window = SDL_CreateWindow("Pixel Buffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_rect.w, screen_rect.h, SDL_WINDOW_SHOWN);
	if(!window) {
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_bool mouse_mode = SDL_TRUE;
	SDL_SetRelativeMouseMode(mouse_mode);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}


	// Create a pixel buffer
	uint buffer_size = (uint) (dimensions.x * dimensions.y);
	Uint32 *screen_buffer = new Uint32[buffer_size];
	float *z_buffer = new float[buffer_size];
	for(int i = 0; i < buffer_size; ++i) {
		screen_buffer[i] = 0xFF0000FF; // Black background color
	}


	// Create a texture from the pixel buffemain
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, (int)dimensions.x, (int)dimensions.y);
	SDL_PixelFormat *pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	//Pixel interpolation
	char quality = 0;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &quality);

	bool running = true;

	uint frameCount = 0;
	auto millisecond = getCurrentMilliseconds();
	auto frameStamp = millisecond;
	Signals signals;

	Vec3f translate = camera->pos;
	Vec3f rotate = camera->rotation;

	while(signals.running) {

		//Clear the buffer
		for(int i = 0; i < dimensions.x * dimensions.y; ++i) {
			//screen_buffer[i] = 0x000000FF; // Bleck color
			z_buffer[i] = 0.0;
		}
		draw_scene(scene, screen_buffer, dimensions, translate, rotate, z_buffer, pixel_format, lights);
		//draw_bounding_boxes(staticBoxes, entities, translate, rotate, dimensions, screen_buffer, z_buffer);


		auto currentFrameMillis = getCurrentMilliseconds();
		if(currentFrameMillis - frameStamp < 1000) {
			frameCount ++;
		} else {
			std::cout << frameCount << " fps" << std::endl;
			//std::cout << "X: " << rotate.x << std::endl;
			//std::cout << "Y: " << rotate.y << std::endl;
			frameStamp = currentFrameMillis;
			frameCount = 0;
		}

		uint diffMillis = currentFrameMillis - millisecond;
		player_tick(camera, signals);
		tick(entities, staticTree, diffMillis);
		camera->rotational_velocity = camera->rotational_velocity * 0.4;
		millisecond = currentFrameMillis;
		if((1000 / diffMillis) > 100) {
			//SDL_Delay(1000/100);
		}

		//Render buffer to the screen
		SDL_UpdateTexture(texture, NULL, screen_buffer, ((uint)dimensions.x) * sizeof(Uint32));
		//SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &screen_rect);
		SDL_RenderPresent(renderer);

		rotate = camera->rotation;
		translate = camera->pos * -1;
		poll_controls(camera, signals);
		box_signals(signals, staticBoxes, camera, staticTree);
	}



	delete[] screen_buffer;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
