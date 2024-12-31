#include <SDL2/SDL.h>
#include <execinfo.h>
#include <signal.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <ctime>

#include "scene.h"
#include "game.h"
#include "boxedit.h"
#include "player.h"
#include "Physics.h"
#include "Light.h"
#include "rasterize.h"




long long getCurrentMilliseconds() {
	using namespace std::chrono;
	auto now = system_clock::now();                 // Get current time as system clock
	auto duration = now.time_since_epoch();         // Get duration since epoch
	return duration_cast<milliseconds>(duration).count(); // Convert to milliseconds
}


void handler(int sig) {
	void *array[10];
	size_t size;
	
	// get void*'s for all entries on the stack
	size = backtrace(array, 10);
	
	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int main(int argc, char* argv[]) {
	signal(SIGSEGV, handler);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	SDL_Rect screen_rect = SDL_Rect{0, 0, int(DM.w*1), int(DM.h*1)};

	Vec2f dimensions = Vec2f{ (float)floor(screen_rect.w/1), (float)floor(screen_rect.h/1) };

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
	std::unordered_map<std::string, SDL_Surface *> texture_pool;
	std::vector<Entity> entities;
	std::vector<Box> staticBoxes;
	scene.reserve(8192);
	texture_pool.reserve(64);
	staticBoxes.reserve(1024);
	build_scene(scene, texture_pool, entities, staticBoxes);
	texture_pool.clear();
	for(Entity &entity : entities) {
		setRotationMatrix(entity, true);	//Initialize rotation matrices
	}

	//std::random_shuffle(staticBoxes.begin(), staticBoxes.end());
	Box *staticTree = nullptr;
	for(Box &box : staticBoxes) {
		staticTree = insert_box(staticTree, box);
	}

	std::vector<Light> lights;
	some_lights(lights);
	light_scene(scene, lights);

	Entity *camera = &(entities[0]);

	Vec3f translate = camera->pos;
	Vec3f rotate = camera->rotation;

	// Create a texture from the pixel buffemain
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, (int)dimensions.x, (int)dimensions.y);


	bool running = true;

	uint frameCount = 0;
	auto millisecond = getCurrentMilliseconds();
	auto frameStamp = millisecond;
	Signals signals;


	while(signals.running) {

		//Clear the buffer
		for(int i = 0; i < dimensions.x * dimensions.y; ++i) {
			//screen_buffer[i] = 0x000000FF; // Bleck color
			z_buffer[i] = 0.0;
		}
		draw_scene(scene, screen_buffer, dimensions, translate, rotate, z_buffer);
		draw_bounding_boxes(staticBoxes, entities, translate, rotate, dimensions, screen_buffer, z_buffer);


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
		//translate = camera->pos * -1;
		//translate.z += cos(rotate.y) * 1.8;
		//translate.x += sin(rotate.y) * 1.8;
		//translate.y += 0.3;
		//translate.x -= cos(rotate.y) * 0.5;
		//translate.z += sin(rotate.y) * 0.5;

		//translate.x += cos(camera->rotation.y) * 3;
		//translate.z -= sin(camera->rotation.y) * 3;


		translate = camera->pos * -1;
		//Fixed Camera Angles
		//translate = Vec3f{6, 8, -7};
		//rotate = Vec3f{ 0.6, 2.5, 0};

		poll_controls(camera, signals);
		box_signals(signals, staticBoxes, camera, staticTree);

	}

	scene.clear();
	texture_pool.clear();
	delete[] screen_buffer;
	for(uint i=0; i<texture_pool.size(); i++) {
		//SDL_DestroyTexture(texture_pool[i]);
		//SDL_DestroyWindowSurface(window);
	}

	entities.clear();
	staticBoxes.clear();

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
