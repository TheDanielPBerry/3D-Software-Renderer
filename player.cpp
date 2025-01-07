
#include <SDL2/SDL_surface.h>
#include <vector>

#include "Plane.h"
#include "model.h"
#include "game.h"


void player_model(std::vector<Plane> &scene, std::unordered_map<std::string, SDL_Surface *> &texture_pool, std::vector<Entity> &entities, std::vector<Box> &staticBoxes)
{
	std::vector<Model> models;
	int shotgun = load_obj_model("assets/models/shotgun.obj", scene, texture_pool, models);
	int rpg = load_obj_model("assets/models/rpg.obj", scene, texture_pool, models);

	//models[rpg].boxes.push_back(models[crate].boxes[0]);

	//ADS RPG
	add_model_to_scene(models[rpg], scene, texture_pool, staticBoxes, Vec3f{ 0.0, 0.22, 0.0}, Vec3f{ -0.15, 3.141592, 3.141592 }, Vec3f{ 0.1, 0.1, 0.1 }, true, &(entities[0]), true);
	//Hip Fire RPG
	add_model_to_scene(models[rpg], scene, texture_pool, staticBoxes, Vec3f{ 0.2, 0.2, 0.3}, Vec3f{ 0, 3.14, 3.18 }, Vec3f{ 0.1, 0.1, 0.1 }, true, &(entities[0]), true);

	//Hip fire shotgun
	add_model_to_scene(models[shotgun], scene, texture_pool, staticBoxes, Vec3f{ 0.2, 0.2, 0.3}, Vec3f{ 0, 3.14/2, 3.18 }, Vec3f{ 0.1, 0.1, 0.08 }, true, &(entities[0]), true);

}


/*
Plane &vec_plane_intersection(std::Vec3f &plane, )
{

}
*/


void player_tick(Entity *camera, Signals &signals)
{
	#define PLAYER_ACCELERATION 4.0
	#define JUMP_HEIGHT -50.0
	#define GOD_MODE true

	if(signals.forward) {
		camera->vel.z += cos(camera->rotation.y) * PLAYER_ACCELERATION;
		camera->vel.x += sin(camera->rotation.y) * PLAYER_ACCELERATION;
	}
	if(signals.back) {
		camera->vel.z -= cos(camera->rotation.y) * PLAYER_ACCELERATION;
		camera->vel.x -= sin(camera->rotation.y) * PLAYER_ACCELERATION;
	}
	if(signals.left) {
		camera->vel.x -= cos(camera->rotation.y) * PLAYER_ACCELERATION;
		camera->vel.z += sin(camera->rotation.y) * PLAYER_ACCELERATION;
	}
	if(signals.right) {
		camera->vel.x += cos(camera->rotation.y) * PLAYER_ACCELERATION;
		camera->vel.z -= sin(camera->rotation.y) * PLAYER_ACCELERATION;
	}
	if(GOD_MODE) {
		if(signals.jump) {
			camera->vel.y -= 1;
		}
		if(signals.crouch) {
			camera->vel.y += 1;
		}
		camera->vel.y *= 0.9;
	} else {
		if(signals.jump) {
			if(camera->grounded) {
				camera->vel.y = JUMP_HEIGHT;
				camera->grounded = false;
			}
		}
	}
}
