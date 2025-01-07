
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Plane.h"
#include "model.h"




void build_scene(std::vector<Plane> &scene, std::unordered_map<std::string, SDL_Surface *> &texture_pool, std::vector<Entity> &entities, std::vector<Box> &staticBoxes)
{
	std::vector<Model> models;
	int shotgun = load_obj_model("assets/models/shotgun.obj", scene, texture_pool, models);
	int chest = load_obj_model("assets/models/chest.obj", scene, texture_pool, models);
	int chest_open = load_obj_model("assets/models/chest_open.obj", scene, texture_pool, models);
	int floor = load_obj_model("assets/models/floor.obj", scene, texture_pool, models);
	int crate = load_obj_model("assets/models/crate.obj", scene, texture_pool, models);
	int rpg = load_obj_model("assets/models/rpg.obj", scene, texture_pool, models);
	int level_one = load_obj_model("assets/models/level_two.obj", scene, texture_pool, models);
	int skybox = load_obj_model("assets/models/skybox.obj", scene, texture_pool, models);
	int wood_step = load_obj_model("assets/models/wood_step.obj", scene, texture_pool, models);
	int torch = load_obj_model("assets/models/torch.obj", scene, texture_pool, models);


	//
	//Level Geometry
	int throne_room = load_obj_model("assets/models/throne_room.obj", scene, texture_pool, models);
	int library = load_obj_model("assets/models/library.obj", scene, texture_pool, models);
	int courtyard = load_obj_model("assets/models/courtyard.obj", scene, texture_pool, models);
	int station = load_obj_model("assets/models/station.obj", scene, texture_pool, models);
	int tunnels = load_obj_model("assets/models/tunnels.obj", scene, texture_pool, models);

	models[chest].boxes.push_back(models[crate].boxes[0]);
	models[rpg].boxes.push_back(models[crate].boxes[0]);
	//add_model_to_scene(models[plane], scene, texture_pool, Vec3f{ 0, 0, 2 }, Vec3f { 0, 0, 0 }, Vec3f{ 1, 1, 1 }, false, nullptr);
	//return;
	entities.reserve(32);
	entities.push_back(Entity {
		.pos = Vec3f{ -3, -4, 0},
		.vel = Vec3f{},
		.drag = Vec3f{0.7, 0.98, 0.7},
		.rotation = Vec3f{},
		.rotational_velocity = Vec3f{},
		.type=Player,
	});
	entities.push_back(Entity {
		.pos = Vec3f{0, -8, 2},
		.vel = Vec3f{ 0, 0, 0 },
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 3.14192, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 0, 0},
		.type=Object
	});
	entities.push_back(Entity {
		.pos = Vec3f{4, -8, 2},
		.vel = Vec3f{0, 0, 0},
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 0, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 1.0, 0},
		.type=Shotgun
	});
	entities.push_back(Entity {
		.pos = Vec3f{-2, -10, 3},
		.vel = Vec3f{0, 0, 0},
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 0, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 0, 0 },
		.type=Object
	});


	add_model_to_scene(models[shotgun], scene, texture_pool, staticBoxes, Vec3f{ 0.2, 0.2, 0.3 }, Vec3f{ 0, 3.14/2, 3.18 }, Vec3f{ 0.1, 0.1, 0.08 }, true, &(entities[0]), true);
	entities[0].boxes[0].pos = Vec3f{ -0.4, -0.2, -0.4 };
	entities[0].boxes[0].dim = Vec3f{ 0.8, 2.4, 0.8 };

	//add_model_to_scene(models[rpg], scene, texture_pool, staticBoxes, Vec3f{ 0.0, 0.22, 0.0}, Vec3f{ -0.15, 3.141592, 3.141592 }, Vec3f{ 0.1, 0.1, 0.1 }, true, &(entities[0]), true);
	add_model_to_scene(models[chest], scene, texture_pool, staticBoxes, Vec3f{}, Vec3f{}, Vec3f{ 0.3, 0.3, 0.3 }, true, &(entities[1]));
	add_model_to_scene(models[rpg], scene, texture_pool, staticBoxes, Vec3f{0, -0.3, 0}, Vec3f{3.14192}, Vec3f{0.1,0.1,0.1}, true, &(entities[2]));

	add_model_to_scene(models[crate], scene, texture_pool, staticBoxes, Vec3f{ }, Vec3f{}, Vec3f{ 0.5, 0.5, 0.5 }, true, &(entities[3]));

	Vec3f pos = Vec3f{ 0, 0, 0 };
	for(uint i=0; i<7; i++) {
		pos.x =	(i * -0.7) - 5;
		pos.y = (i * -0.3);
		add_model_to_scene(models[wood_step], scene, texture_pool, staticBoxes, pos, Vec3f{}, Vec3f{0.4,0.4,1.5}, true, nullptr);
	}

	add_model_to_scene(models[level_one], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);

	//Scene Geometry
	add_model_to_scene(models[tunnels], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);
	add_model_to_scene(models[throne_room], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);
	add_model_to_scene(models[station], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);
	add_model_to_scene(models[courtyard], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);
	add_model_to_scene(models[library], scene, texture_pool, staticBoxes, Vec3f{0,0,0}, Vec3f{}, Vec3f{ 1, 1, 1 }, true, nullptr);

	//Torches Lights
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-5.0,-7,-6.48849}, Vec3f{0,0.1,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-5.0,-7,-10.48849}, Vec3f{0,0.1,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-5.0,-7,-14.48849}, Vec3f{0,0.1,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);

	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{4.9,-7,-7.0}, Vec3f{0,3.2,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{4.9,-7,-11.0}, Vec3f{0,3.2,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{4.9,-7,-15.0}, Vec3f{0,3.2,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);


	//Courtyard Lights
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-17.7341,-7.34089,-20.8009}, Vec3f{0,1.6,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-24.533,-9.79371,11.8}, Vec3f{0,-1.6,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);

	//Library Lights
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{10.347,-5.72349,-26.2754}, Vec3f{0,3.2,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-12.4549,-4.77474,-42.0326}, Vec3f{0,1.6,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);


	//Station Light
	add_model_to_scene(models[torch], scene, texture_pool, staticBoxes, Vec3f{-50.0712,-10.9387,-12.6789}, Vec3f{0,0.2,0}, Vec3f{ 0.1, 0.1, 0.1 }, true, nullptr);
	//add_model_to_scene(models[floor], scene, texture_pool, staticBoxes, Vec3f{0, 1, 0}, Vec3f{}, Vec3f{ 0.5, 1.0, 0.5 }, true, nullptr);
	//return;
	add_model_to_scene(models[skybox], scene, texture_pool, staticBoxes, Vec3f{}, Vec3f{2,0,0}, Vec3f{ 8, 8, 8 }, true, nullptr);
	

	std::cout << "Scene triangle count: " << scene.size() << std::endl;
}
