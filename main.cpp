#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "rasterize.h"

void build_scene(std::vector<Plane> &scene)
{
	for(float x=-10; x<10; x++) {
		for(float z=1; z<10; z++) {
			//Bottom Face
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), 1.0, (float)(z - 0.5)},
					{(float)(x + -0.5), 1.0, (float)(z - 0.5)},
					{(float)(x + -0.5), 1.0, (float)(z + 0.5)},
				},
				.color =0xFF00FFFF
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), 1.0, (float)(z + -0.5)},
					{(float)(x + 0.5), 1.0,  (float)(z + 0.5)},
					{(float)(x + -0.5), 1.0,  (float)(z + 0.5)},
				},
				.color = 0x00FFFFFF
			});
		}
	}
}


int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	uint SCREEN_WIDTH = 1024;
	uint SCREEN_HEIGHT = 800;

	SDL_Window* window = SDL_CreateWindow("Pixel Buffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}


	// Create a pixel buffer
	Uint32 *screen_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	float *z_buffer = new float[SCREEN_WIDTH * SCREEN_HEIGHT];
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		screen_buffer[i] = 0xFF0000FF; // Red color
	}
	Vec2f dimensions = Vec2f{ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };


	std::vector<Plane> scene;
	scene.reserve(360);
	build_scene(scene);
	Vec3f transform = Vec3f{0,0,0};
	draw_scene(scene, screen_buffer, dimensions, transform, z_buffer);

	// Create a texture from the pixel buffer
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_UpdateTexture(texture, NULL, screen_buffer, SCREEN_WIDTH * sizeof(Uint32));

	// Render the texture
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// Event loop
	SDL_Event event;
	bool running = true;
	while (running) {
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
			screen_buffer[i] = 0xFF0000FF; // Red color
			z_buffer[i] = INFINITY;
		}
		draw_scene(scene, screen_buffer, dimensions, transform, z_buffer);

		SDL_UpdateTexture(texture, NULL, screen_buffer, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.scancode  == 5) {
						std::cout << "Debugger? I hardly know her.";
					} else if(event.key.keysym.scancode  == 41) {
						running = false;
					} else if(event.key.keysym.scancode  == 26 || event.key.keysym.scancode == 14) {
						transform.z -= 0.2;
					} else if(event.key.keysym.scancode  == 22 || event.key.keysym.scancode == 13) {
						transform.z += 0.2;
					} else if(event.key.keysym.scancode  == 11 || event.key.keysym.scancode == 4) {
						transform.x += 0.2;
					} else if(event.key.keysym.scancode  == 15 || event.key.keysym.scancode == 7) {
						transform.x -= 0.2;
					} else if(event.key.keysym.scancode  == 44) {
						transform.y += 0.2;
					} else if(event.key.keysym.scancode  == 225) {
						transform.y -= 0.2;
					} else {
						std::cout << event.key.keysym.scancode << std::endl;
					}
					break;
			}
		}
	}

	scene.clear();
	delete[] screen_buffer;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
