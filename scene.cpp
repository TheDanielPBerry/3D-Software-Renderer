
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Plane.h"

/**
* @return identifier to texture
*/
std::pair<SDL_Surface *, uint> load_texture(const char *filePath, std::vector<SDL_Surface *> &texture_pool)
{
	SDL_Surface *texture = IMG_Load(filePath);
	SDL_LockSurface(texture);
	texture_pool.push_back(texture);
	return std::make_pair(texture, texture_pool.size()-1);
}


void build_scene(std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool)
{
	std::pair<SDL_Surface *, uint> brick = load_texture("assets/bricks.png", texture_pool);
	float x = 0.0, z= 0.0, y  = 0.0;
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
	scene.push_back(Plane{
		.points = {
			{(float)(x + 0.5 - 0), (float)(y + 0.5), (float)(x + 0.5) + 2},
			{(float)(x - 0.5 - 0), (float)(y + 0.5), (float)(x - 0.5) + 2},
			{(float)(x - 0.5 - 0), (float)(y - 0.5), (float)(x - 0.5) + 2},
		},
		.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
		.texture_coords = {{ 1, 1 }, { 0, 1 }, { 0 , 0 } },
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
				.texture = brick.first,
			});
			scene.push_back(Plane{
				.points = {
					{(float)(x + 0.5), 1, (float)(z + -0.5)},
					{(float)(x + 0.5), 1,  (float)(z + 0.5)},
					{(float)(x + -0.5), 1,  (float)(z + 0.5)},
				},
				.color = {{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, 1.0}},
				.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 }},
				.texture = brick.first,
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
