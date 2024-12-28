
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <vector>

#include "Plane.h"
#include "model.h"



void build_scene(std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool, std::vector<Entity> &entities, std::vector<Box> &staticBoxes)
{
	std::vector<Model> models;
	int shotgun = load_obj_model("assets/models/shotgun.obj", scene, texture_pool, models);
	int chest = load_obj_model("assets/models/chest.obj", scene, texture_pool, models);
	int chest_open = load_obj_model("assets/models/chest_open.obj", scene, texture_pool, models);
	int cube = load_obj_model("assets/models/cube.obj", scene, texture_pool, models);
	int plane = load_obj_model("assets/models/plane.obj", scene, texture_pool, models);
	int floor = load_obj_model("assets/models/floor.obj", scene, texture_pool, models);
	int crate = load_obj_model("assets/models/crate.obj", scene, texture_pool, models);
	int rpg = load_obj_model("assets/models/rpg.obj", scene, texture_pool, models);
	int globe = load_obj_model("assets/models/globe.obj", scene, texture_pool, models);

	models[shotgun].boxes.push_back(models[crate].boxes[0]);
	models[chest].boxes.push_back(models[crate].boxes[0]);
	models[rpg].boxes.push_back(models[crate].boxes[0]);
	//add_model_to_scene(models[plane], scene, texture_pool, Vec3f{ 0, 0, 2 }, Vec3f { 0, 0, 0 }, Vec3f{ 1, 1, 1 }, false, nullptr);
	//return;
	entities.reserve(8);
	entities.push_back(Entity {
		.pos = Vec3f{},
		.vel = Vec3f{0,-5,0},
		.drag = Vec3f{0.7, 0.8, 0.7},
		.rotation = Vec3f{},
		.rotational_velocity = Vec3f{},
		.name = "player",
	});
	entities.push_back(Entity {
		.pos = Vec3f{0, -8, 2},
		.vel = Vec3f{ 0, 0, 0 },
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 3.14192, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 0, 0},
		.name = "chest",
	});
	entities.push_back(Entity {
		.pos = Vec3f{4, -8, 2},
		.vel = Vec3f{0, 0, 0},
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 3.14192, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 0.02, 0},
		.name = "shotgun",
	});
	entities.push_back(Entity {
		.pos = Vec3f{-2, -10, 3},
		.vel = Vec3f{0, 0, 0},
		.drag = Vec3f{0.9, 0.9, 0.9},
		.rotation = Vec3f{ 0, 0, 0 },
		.rotational_velocity = Vec3f{ 0, 0, 0 },
		.name = "crate",
	});


	add_model_to_scene(models[shotgun], scene, texture_pool, Vec3f{ 0.2, 0.2, 0.3}, Vec3f{ 0, 3.14/2, 3.18 }, Vec3f{ 0.1, 0.1, 0.08 }, true, &(entities[0]), true);
	//add_model_to_scene(models[rpg], scene, texture_pool, Vec3f{ 0.0, 0.22, 0.0}, Vec3f{ -0.15, 3.141592, 3.141592 }, Vec3f{ 0.1, 0.1, 0.1 }, true, &(entities[0]), true);
	add_model_to_scene(models[chest], scene, texture_pool, Vec3f{}, Vec3f{}, Vec3f{ 0.3, 0.3, 0.3 }, true, &(entities[1]));
	add_model_to_scene(models[rpg], scene, texture_pool, Vec3f{}, Vec3f{}, Vec3f{ 0.06, 0.06, 0.06 }, true, &(entities[2]));

	add_model_to_scene(models[crate], scene, texture_pool, Vec3f{ }, Vec3f{}, Vec3f{ 0.5, 0.5, 0.5 }, true, &(entities[3]));

	Vec3f crateStatic = entities[2].pos;
	crateStatic.y = 1.2;
	for(int z = -10; z<10; z++) {
		for(int x = -10; x<10; x++) {
			crateStatic.x = x;
			crateStatic.z = z;
			for(Box b : models[crate].boxes) {
				b.pos = b.pos * Vec3f{ 0.5, 0.5, 0.5 };
				b.pos = b.pos + crateStatic;
				b.dim = b.dim * Vec3f{ 0.5, 0.5, 0.5 };
				staticBoxes.push_back(b);
			}
			//add_model_to_scene(models[crate], scene, texture_pool, crateStatic, Vec3f{}, Vec3f{ 0.5, 0.5, 0.5 }, true, nullptr);
		}
	}
	add_model_to_scene(models[globe], scene, texture_pool, Vec3f{5, -2, -10}, Vec3f{}, Vec3f{ 0.5, 0.5, 0.5 }, true, nullptr);
	add_model_to_scene(models[floor], scene, texture_pool, Vec3f{0, 1, 0}, Vec3f{}, Vec3f{ 0.5, 1.0, 0.5 }, true, nullptr);
	//return;

	std::pair<SDL_Surface *, uint> brick = load_texture("assets/bricks.png", texture_pool);
	float x = 0.0, z= 0.0, y  = 0.0;
	scene.push_back(Plane{
		.points = {
			{(float)(x + 0.5 - 0), (float)(y + 0.5),  2},
			{(float)(x - 0.5 - 0), (float)(y + 0.5),  2},
			{(float)(x - 0.5 - 0), (float)(y - 0.5),  2},
		},
		.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
		.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
		.texture = brick.first,
	});
	scene.push_back(Plane{
		.points = {
			{(float)(x + 0.5 - 0), (float)(y + 0.5), (float)(x + 0.5) + 2},
			{(float)(x + 0.5 - 0), (float)(y - 0.5), (float)(x + 0.5) + 2},
			{(float)(x - 0.5 - 0), (float)(y - 0.5), (float)(x - 0.5) + 2},
		},
		.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
		.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
		.texture = brick.first,
	});

	for(float x=-5; x<5; x+=1) {
		for(float z=-5; z<5; z+=1) {
			//add_model_to_scene(models[floor], scene, texture_pool, Vec3f{ float(x*6.59), 1.0, float(z*6.59) }, Vec3f { 0, 0, 0 }, Vec3f{ 0.1, 1, 0.1 }, true, nullptr);
		}
	}
	//std::pair<SDL_Surface *, uint> floor = load_texture("assets/floor.png", texture_pool);
	//		//Bottom Face
	//		scene.push_back(Plane{
	//			.points = {
	//				{(float)(x + 0.5), 1, (float)(z - 0.5)},
	//				{(float)(x + -0.5), 1, (float)(z - 0.5)},
	//				{(float)(x + -0.5), 1, (float)(z + 0.5)},
	//			},
	//			.color = {{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0}},
	//			.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 }},
	//			.texture = floor.first,
	//		});
	//		scene.push_back(Plane{
	//			.points = {
	//				{(float)(x + 0.5), 1, (float)(z + -0.5)},
	//				{(float)(x + 0.5), 1,  (float)(z + 0.5)},
	//				{(float)(x + -0.5), 1,  (float)(z + 0.5)},
	//			},
	//			.color = {{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0}},
	//			.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 }},
	//			.texture = floor.first,
	//		});
	//	}
	//}


	for(float x=-2; x<2; x++) {
		for(float y=-2; y<2; y++) {
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), (float)(y + 0.5), 9},
					{(float)(x + 0.5), (float)(y - 0.5), 9},
					{(float)(x - 0.5), (float)(y - 0.5), 9},
				},
				.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), (float)(y + 0.5), 9},
					{(float)(x - 0.5), (float)(y + 0.5), 9},
					{(float)(x - 0.5), (float)(y - 0.5), 9},
				},
				.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
				.texture = brick.first,
			});


			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 + 2), (float)(y + 0.5), 11},
					{(float)(x + 0.5 + 2), (float)(y - 0.5), 11},
					{(float)(x - 0.5 + 2), (float)(y - 0.5), 11},
				},
				.color = {{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 + 2), (float)(y + 0.5), 11},
					{(float)(x - 0.5 + 2), (float)(y + 0.5), 11},
					{(float)(x - 0.5 + 2), (float)(y - 0.5), 11},
				},
				.color = {{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
				.texture = brick.first,
			});

			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 - 2), (float)(y + 0.5), 13},
					{(float)(x + 0.5 - 2), (float)(y - 0.5), 13},
					{(float)(x - 0.5 - 2), (float)(y - 0.5), 13},
				},
				.color = {{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 - 2), (float)(y + 0.5), 13},
					{(float)(x - 0.5 - 2), (float)(y + 0.5), 13},
					{(float)(x - 0.5 - 2), (float)(y - 0.5), 13},
				},
				.color = {{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
				.texture = brick.first,
			});

			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 + 5), (float)(y + 0.5), (float)(x + 0.5) + 7},
					{(float)(x + 0.5 + 5), (float)(y - 0.5), (float)(x + 0.5) + 7},
					{(float)(x - 0.5 + 5), (float)(y - 0.5), (float)(x - 0.5) + 7},
				},
				.color = {{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 + 5), (float)(y + 0.5), (float)(x + 0.5) + 7},
					{(float)(x - 0.5 + 5), (float)(y + 0.5), (float)(x - 0.5) + 7},
					{(float)(x - 0.5 + 5), (float)(y - 0.5), (float)(x - 0.5) + 7},
				},
				.color = {{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
				.texture = brick.first,
			});

			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 - 4), (float)(y + 0.5), (float)(x + 0.5) + 13},
					{(float)(x + 0.5 - 4), (float)(y - 0.5), (float)(x + 0.5) + 13},
					{(float)(x - 0.5 - 4), (float)(y - 0.5), (float)(x - 0.5) + 13},
				},
				.color = {{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5 - 4), (float)(y + 0.5), (float)(x + 0.5) + 13},
					{(float)(x - 0.5 - 4), (float)(y + 0.5), (float)(x - 0.5) + 13},
					{(float)(x - 0.5 - 4), (float)(y - 0.5), (float)(x - 0.5) + 13},
				},
				.color = {{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0},{0.0, 1.0, 0.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
				.texture = brick.first,
			});
	 	}
	}
	std::cout << "Scene triangle count: " << scene.size() << std::endl;
}
