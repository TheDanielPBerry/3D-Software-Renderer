#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <SDL2/SDL_surface.h>

#include "Plane.h"

void load_obj_model(std::string filePath, std::vector<Plane> &scene, Entity *entity, std::vector<SDL_Surface *> &texture_pool);

#endif
