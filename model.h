#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <SDL2/SDL_surface.h>

#include "Plane.h"

typedef struct Model {
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texture_coords;
	std::vector<std::vector<std::vector<int>>> planes;
	int texture;
} Model;


std::pair<SDL_Surface *, uint> load_texture(const char *filePath, std::vector<SDL_Surface *> &texture_pool);

int load_obj_model(std::string filePath, std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool, std::vector<Model> &models);

void add_model_to_scene(Model &model, std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool, Vec3f pos, Vec3f rotation, Vec3f scale);

#endif
