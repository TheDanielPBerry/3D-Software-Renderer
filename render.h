#include <iostream>

#include <SDL2/SDL.h>

#include "geo/geo2d.h"

struct Buffer {
	uint dimensions[2];
	Uint32 (*pixels);
};

void fill_triangles(Buffer *buffer, const float (*data), const uint NUM_TRIANGLES);
void draw_triangle(struct Buffer *buffer, geo2d::triangle2f *t, uint color);
