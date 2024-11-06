#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <iostream>

#include "render.h"


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

	/*
	geo2d::triangle2f triangle;
	triangle.points[0].axes[0] = 0.25;
	triangle.points[0].axes[1] = 0.75;

	triangle.points[1].axes[0] = 0.5;
	triangle.points[1].axes[1] = 0.25;

	triangle.points[2].axes[0] = 0.75;
	triangle.points[2].axes[1] = 0.75;
	draw_triangle(&buffer, &triangle, 0x0000FFFF);
	*/
	#define N_T 2000

	float triangles[N_T * 24] = {
		0.5, 0.25, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0,
		0.25, 0.75, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0,
		0.75, 0.75, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0
	};
	for(uint i=24; i<N_T*24; i++) {
		triangles[i] = drand48();
	}
	fill_triangles(&buffer, triangles, N_T);

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
			pixels[i] = 0x000000FF; // Red color
		}
		fill_triangles(&buffer, triangles, N_T);
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
					for(uint i=0; i<N_T; i++) {
						triangles[16 + (i*24)] += ((0.0 + event.motion.xrel)/WIDTH);
						triangles[17 + (i*24)] += ((0.0 + event.motion.yrel)/HEIGHT);
					}
					break;
				case SDL_KEYDOWN:
					std::cout << event.key.keysym.scancode << std::endl;
					if(event.key.keysym.scancode  == 5) {
						std::cout << "Debugger? I hardly know her.";
					}
					break;
			}
		}
	}

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
