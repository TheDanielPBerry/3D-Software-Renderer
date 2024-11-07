#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "render.h"
#include "camera.h"


uint buffer[1920][1080];

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	const int WIDTH = 1024;
	const int HEIGHT = 800;

	SDL_Window* window = SDL_CreateWindow("Pixel Buffer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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
	Uint32* pixels = new Uint32[WIDTH * HEIGHT];
	for (int i = 0; i < WIDTH * HEIGHT; ++i) {
		pixels[i] = 0xFF0000FF; // Red color
	}


	Buffer buffer;
	buffer.dimensions[0] = WIDTH;
	buffer.dimensions[1] = HEIGHT;
	buffer.pixels = pixels;

	std::vector<Plane> chunk;
	build_cube(chunk);

	float transform[3] = {0,0,0};


	const uint n_triangles = chunk.size();
	std::vector<float> triangles(24 * n_triangles);
	for(uint i=0; i<n_triangles; i++) {
		project(&(chunk[i]), &(triangles[i*24]), transform);
	}
	fill_triangles(&buffer, triangles, n_triangles);

	// Create a texture from the pixel buffer
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
	SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

	// Render the texture
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	// Event loop
	SDL_Event event;
	bool running = true;
	while (running) {
		for (int i = 0; i < WIDTH * HEIGHT; ++i) {
			pixels[i] = 0xFF0000FF; // Red color
		}
		for(uint i=0; i<n_triangles; i++) {
			project(&chunk[i], &(triangles[i*24]), transform);
		}
		fill_triangles(&buffer, triangles, n_triangles);
		SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
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
						transform[2] -= 0.2;
					} else if(event.key.keysym.scancode  == 22 || event.key.keysym.scancode == 13) {
						transform[2] += 0.2;
					} else if(event.key.keysym.scancode  == 11 || event.key.keysym.scancode == 4) {
						transform[0] += 0.2;
					} else if(event.key.keysym.scancode  == 15 || event.key.keysym.scancode == 7) {
						transform[0] -= 0.2;
					} else if(event.key.keysym.scancode  == 44) {
						transform[1] += 0.2;
					} else if(event.key.keysym.scancode  == 225) {
						transform[1] -= 0.2;
					} else {
						std::cout << event.key.keysym.scancode << std::endl;
					}
					break;
			}
		}
	}

	chunk.clear();
	triangles.clear();
	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
