#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SDL2/SDL.h>

#include "Plane.h"

void build_scene(std::vector<Plane> &scene, std::vector<SDL_Surface *> &texture_pool);
std::pair<SDL_Surface *, uint> load_texture(const char *filePath, std::vector<SDL_Surface *> &texture_pool);

#endif
