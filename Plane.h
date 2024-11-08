#include <SDL2/SDL.h>

#include "Texture.h"

typedef struct Vec2f {
	float y;
	float x;

	Vec2f operator+(const Vec2f &b)
	{
		return Vec2f {
			x + b.x,
			y + b.y
		};
	}
} Vec2f;

typedef struct Vec3f {
	float x;
	float y;
	float z;

	Vec3f operator+(const Vec3f &b)
	{
		return Vec3f {
			x + b.x,
			y + b.y,
			z + b.z
		};
	}
} Vec3f;


#define N_POINTS 3

typedef struct Plane {
	Vec3f points[N_POINTS];
	Vec3f buffer[N_POINTS];
	Uint32 color;
	Vec2f texture_coords[N_POINTS];
	Texture *texture;
} Plane;
