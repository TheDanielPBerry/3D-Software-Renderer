#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <ctime>

#include "scene.h"
#include "Physics.h"
#include "Light.h"
#include "rasterize.h"

long long getCurrentMilliseconds() {
	using namespace std::chrono;
	auto now = system_clock::now();                 // Get current time as system clock
	auto duration = now.time_since_epoch();         // Get duration since epoch
	return duration_cast<milliseconds>(duration).count(); // Convert to milliseconds
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Rect screen_rect = SDL_Rect{0, 0, int(DM.w*0.8), int(DM.h*0.8)};

	Vec2f dimensions = Vec2f{ (float)floor(screen_rect.w/2), (float)floor(screen_rect.h/2) };

	SDL_Window* window = SDL_CreateWindow("Pixel Buffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_rect.w, screen_rect.h, SDL_WINDOW_SHOWN);
	if (!window) {
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


	std::vector<Plane> scene;
	std::vector<SDL_Surface *> texture_pool;
	std::vector<Entity> entities;
	std::vector<Box> staticBoxes;
	build_scene(scene, texture_pool, entities, staticBoxes);
	for(Entity &entity : entities) {
		setRotationMatrix(entity, true);	//Initialize rotation matrices
	}

	std::random_shuffle(staticBoxes.begin(), staticBoxes.end());
	Box *staticTree;
	for(Box &box : staticBoxes) {
		staticTree = insert_box(staticTree, box);
	}

	std::vector<Light> lights;
	some_lights(lights);
	light_scene(scene, lights);

	Entity *camera = &(entities[0]);
	camera->boxes[0].pos = camera->boxes[0].pos * Vec3f{10, 10, 10};
	camera->boxes[0].dim = camera->boxes[0].dim * Vec3f{10, 10, 10};
	camera->boxes[0].dim.y *= 1.4;

	Vec3f translate = camera->pos;
	Vec3f rotate = camera->rotation;
	draw_scene(scene, screen_buffer, dimensions, translate, rotate, z_buffer);

	// Create a texture from the pixel buffer
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, (int)dimensions.x, (int)dimensions.y);
	SDL_UpdateTexture(texture, NULL, screen_buffer, ((uint)dimensions.x)  * sizeof(Uint32));

	// Render the texture
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// Event loop
	SDL_Event event;
	bool running = true;

	uint frameCount = 0;
	float speed = 0.2;
	auto timestamp = std::time(nullptr);
	auto millisecond = getCurrentMilliseconds();
	while (running) {
		for (int i = 0; i < dimensions.x * dimensions.y; ++i) {
			screen_buffer[i] = 0x000000FF; // Bleck color
			z_buffer[i] = 0.0;
		}
		draw_scene(scene, screen_buffer, dimensions, translate, rotate, z_buffer);
		if(std::time(nullptr) == timestamp) {
			frameCount ++;
		} else {
			std::cout << frameCount <<" fps" << std::endl;
			//std::cout << "X: " << rotate.x << std::endl;
			//std::cout << "Y: " << rotate.y << std::endl;
			timestamp = std::time(nullptr);
			frameCount = 0;
		}
		if(frameCount % 3 == 0) {
			auto currentFrameMillis = getCurrentMilliseconds();
			if(currentFrameMillis - millisecond > 10) {
				tick(entities, staticTree);
				camera->rotational_velocity = camera->rotational_velocity * Vec3f{ 0.1, 0.1, 0.1 };
				millisecond = currentFrameMillis;
			}
		}

		SDL_UpdateTexture(texture, NULL, screen_buffer, ((uint)dimensions.x) * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &screen_rect);
		SDL_RenderPresent(renderer);
		translate = camera->pos * Vec3f{ -1, -1, -1 };
		rotate = camera->rotation;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEWHEEL:
				speed *= event.wheel.y > 0 ? 2 : 0.5;
				break;
			case SDL_MOUSEMOTION:
				camera->rotational_velocity.y += 0.05 * event.motion.xrel;
				#define PI_OVER_TWO 1.41
				if(event.motion.yrel > 0 && camera->rotation.x < PI_OVER_TWO) {
					camera->rotational_velocity.x += 0.05 * event.motion.yrel;
				} else if(event.motion.yrel < 0 && camera->rotation.x > -PI_OVER_TWO) {
					camera->rotational_velocity.x += 0.05 * event.motion.yrel;
				}
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.scancode == 5) {
					std::cout << "Debugger? I hardly know her.";
				} else if(event.key.keysym.scancode == 41) {
					running = false;
				} else if(event.key.keysym.scancode == 24) {
					mouse_mode = mouse_mode == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
					SDL_SetRelativeMouseMode(mouse_mode);
				} else if(event.key.keysym.scancode == 26 || event.key.keysym.scancode == 14) {
					camera->vel.z += cos(rotate.y) * speed;
					camera->vel.x += sin(rotate.y) * speed;
				} else if(event.key.keysym.scancode == 22 || event.key.keysym.scancode == 13) {
					camera->vel.z -= cos(rotate.y) * speed;
					camera->vel.x -= sin(rotate.y) * speed;
				} else if(event.key.keysym.scancode == 11 || event.key.keysym.scancode == 4) {
					camera->vel.x -= cos(rotate.y) * speed;
					camera->vel.z += sin(rotate.y) * speed;
				} else if(event.key.keysym.scancode == 15 || event.key.keysym.scancode == 7) {
					camera->vel.x += cos(rotate.y) * speed;
					camera->vel.z -= sin(rotate.y) * speed;
				} else if(event.key.keysym.scancode == 44) {
					camera->vel.y = -1;
				} else if(event.key.keysym.scancode == 225) {
					camera->vel.y = -1;
				} else if(event.key.keysym.scancode == 79) {
					//rotate.y += speed;
				} else if(event.key.keysym.scancode == 80) {
					rotate.y -= speed;
				} else {
					std::cout << event.key.keysym.scancode << std::endl;
				}
				break;
			}
		}
	}

	scene.clear();
	texture_pool.clear();
	delete[] screen_buffer;
	for(uint i=0; i<texture_pool.size(); i++) {
		//SDL_DestroyTexture(texture_pool[i]);
		//SDL_DestroyWindowSurface(window);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
