#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>


#include "boxedit.h"
#include "Physics.h"
#include "Vec.h"


#define N_POINTS 2
	
typedef struct Line {
	Vec3f points[2];
	Vec3f buffer[2];
	Box *box;
} Line;


void draw_line(Line &subject, const Vec2f &dimensions, Uint32 *buffer, float *z_buffer)
{
	if(subject.buffer[0].x > dimensions.x && subject.buffer[1].x > dimensions.x) {
		return;
	}
	if(subject.buffer[0].y > dimensions.y && subject.buffer[1].y > dimensions.y) {
		return;
	}
	if(subject.buffer[0].x < 0 && subject.buffer[1].x < 0) {
		return;
	}
	if(subject.buffer[0].y < 0 && subject.buffer[1].y < 0) {
		return;
	}

	Vec2f x_coeffs, z_coeffs;
	coeffs(Vec2f{subject.buffer[0].x, subject.buffer[0].y}, 
		Vec2f{subject.buffer[1].x, subject.buffer[1].y}, x_coeffs);

	coeffs(Vec2f{1/subject.buffer[0].z, subject.buffer[0].y}, 
		Vec2f{1/subject.buffer[1].z, subject.buffer[1].y}, z_coeffs);

	//Get start and end points, then make sure they fit within the bounds of the screen space
	float start = std::min(subject.buffer[0].y, subject.buffer[1].y);
	float end = std::max(subject.buffer[0].y, subject.buffer[1].y);
	start = std::min((float) dimensions.y, std::max((float) 0, start));
	end = std::min((float) dimensions.y, std::max((float) 0, end));

	for(uint y=start; y<=end; y++) {
		uint yOffset = y * dimensions.x;

		int x = line(x_coeffs, y);
		x = std::min((int) dimensions.x-1, std::max((int)0, x));

		float z = line(z_coeffs, y);

		if(z_buffer[yOffset + x] < z) {
			if(subject.box == get_box_of_interest()) {
				buffer[yOffset + x] = 0x0000FF00;
			} else {
				buffer[yOffset + x] = 0xFF000000;
			}
			z_buffer[yOffset + x] = z;
		}
	}
}

void project_line(Line &line, const Vec2f &dimensions)
{
	for(u_char p=0; p<N_POINTS; p++) {
		float z = line.buffer[p].z;
		line.buffer[p].x = ((line.buffer[p].x /z) + 0.5) * dimensions.x;
		line.buffer[p].y = ((line.buffer[p].y /z) + 0.5) * dimensions.y;
	}
}


/*
* Orient line into camera space and apply any transforms
*/
bool transform_line(Line &subject, const Vec3f &translate, Vec3f (&rotationTrig)[2])
{

	#define COSINE 0
	#define SINE 1

	for(u_char p=0; p<N_POINTS; p++) {
		Entity *entity = subject.box->entity;
		Vec3f bufferSpace = subject.points[p] + translate;

		if(entity != nullptr) {
			bufferSpace = bufferSpace + entity->pos;
		}
		rotate(bufferSpace, rotationTrig);

		if(bufferSpace.z < 0.01) {
			return false;
		}
		subject.buffer[p] = bufferSpace;

	}
	return true;
}


