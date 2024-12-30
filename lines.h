#ifndef LINES_H
#define LINES_H

#include <SDL2/SDL.h>
#include "Physics.h"
#include "Vec.h"


typedef struct Line {
	Vec3f points[2];
	Vec3f buffer[2];
	Box *box;
} Line;

void draw_line(Line &subject, const Vec2f &dimensions, Uint32 *buffer, float *z_buffer);

void project_line(Line &line, const Vec2f &dimensions);

bool transform_line(Line &subject, const Vec3f &translate, Vec3f (&rotationTrig)[2]);


#endif
