#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>

#include "Plane.h"

void build_scene(std::vector<Plane> &scene, std::unordered_map<std::string, SDL_Surface *> &texture_pool, std::vector<Entity> &entities, std::vector<Box> &staticBoxes);

#endif
