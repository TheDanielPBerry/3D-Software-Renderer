#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL_surface.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Plane.h"

typedef struct Face {
	SDL_Surface *texture;
	std::vector<std::vector<int>> points;
} Face;

typedef struct Model {
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texture_coords;
	struct std::vector<Face> faces;
	std::vector<Box> boxes;
} Model;


std::pair<SDL_Surface *, uint> load_texture(const char *filePath);

int load_obj_model(std::string filePath, std::vector<Plane> &scene, std::unordered_map<std::string, SDL_Surface *> &texture_pool, std::vector<Model> &models);

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::unordered_map<std::string, SDL_Surface *> &texture_pool,
	std::vector<Box> &staticBoxes,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale,
	bool cullable,
	Entity *entity,
	bool cameraStatic
);

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::unordered_map<std::string, SDL_Surface *> &texture_pool,
	std::vector<Box> &staticBoxes,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale,
	bool cullable,
	Entity *entity
);

#endif
