
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <vector>

#include "Plane.h"
#include "model.h"



void build_scene(std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool)
{
	std::vector<Model> models;
	int shotgun = load_obj_model("assets/models/shotgun.obj", scene, texture_pool, models);
	int chest = load_obj_model("assets/models/chest.obj", scene, texture_pool, models);
	int chest_open = load_obj_model("assets/models/chest_open.obj", scene, texture_pool, models);
	int cube = load_obj_model("assets/models/cube.obj", scene, texture_pool, models);
	add_model_to_scene(models[cube], scene, texture_pool, Vec3f{ 1, 0.68, -2 }, Vec3f { 0, -3.14, 0 }, Vec3f{ 0.1, 0.1, 0.1 }, true);
	add_model_to_scene(models[chest], scene, texture_pool, Vec3f{ -1, 0.68, 1 }, Vec3f { 0, -3.14/2, 0 }, Vec3f{ 0.3, 0.3, 0.3 }, true);
	add_model_to_scene(models[chest_open], scene, texture_pool, Vec3f{ -1, 0.68, -1 }, Vec3f { 0, -3.14/2, 0 }, Vec3f{ 0.3, 0.3, 0.3 }, true);

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

	std::pair<SDL_Surface *, uint> floor = load_texture("assets/floor.png", texture_pool);
	for(float x=-5; x<5; x+=1) {
		for(float z=-5; z<20; z+=1) {
			//Bottom Face
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), 1, (float)(z - 0.5)},
					{(float)(x + -0.5), 1, (float)(z - 0.5)},
					{(float)(x + -0.5), 1, (float)(z + 0.5)},
				},
				.color = {{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 }},
				.texture = floor.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), 1, (float)(z + -0.5)},
					{(float)(x + 0.5), 1,  (float)(z + 0.5)},
					{(float)(x + -0.5), 1,  (float)(z + 0.5)},
				},
				.color = {{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 }},
				.texture = floor.first,
			});
		}
	}


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
