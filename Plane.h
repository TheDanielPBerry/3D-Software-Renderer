#ifndef Plane_h
#define Plane_h

#include <SDL2/SDL.h>


typedef struct Vec2f {
	float x;
	float y;

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

	Vec3f operator+(const Vec3f &b);
	Vec3f operator*(const Vec3f &b);

} Vec3f;

typedef struct Vec4f {
	float x;
	float y;
	float z;
	float w;
} Vec4f;


#define N_POINTS 3

typedef struct Plane {
	Vec3f points[N_POINTS];
	Vec3f buffer[N_POINTS];
	Vec4f color[N_POINTS];
	Vec2f texture_coords[N_POINTS];
	SDL_Surface *texture;
} Plane;


Vec2f ident(const Vec3f t, const char type);

float line(Vec2f &coeff, float x);

void check_infinity(Vec2f &coeffs, const float target);

void coeffs(const Vec2f &a, const Vec2f &b, Vec2f &dest);

float distance_squared(const Vec3f &a, const Vec3f &b);

void transform(Plane &plane, const Vec3f &translate, const Vec3f &rotate);

/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_and_scale(Plane &plane, const Vec2f &dimensions);

#endif
