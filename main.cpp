#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <execinfo.h>
#include <signal.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "scene.h"
#include "Physics.h"
#include "Light.h"
#include "game.h"






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



int online_connection()
{
	return 0;
}


int main(int argc, char* argv[]) {
	signal(SIGSEGV, handler);


	//Setup Single Player Game State
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

	std::random_shuffle(staticBoxes.begin(), staticBoxes.end());
	Box *staticTree = nullptr;
	for(Box &box : staticBoxes) {
		staticTree = insert_box(staticTree, box, 0);
	}

	std::vector<Light> lights;
	some_lights(lights);
	light_scene(scene, lights);

	Entity *camera = &(entities[0]);



	int finish = init_game(scene, camera, staticTree, entities, lights, staticBoxes);

	entities.clear();
	scene.clear();
	staticBoxes.clear();
	return finish;
}
