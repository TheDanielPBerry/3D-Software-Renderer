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
} Plane;



/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_transform_and_scale(Plane &plane, const Vec3f &transform, const Vec2f &dimensions);
void project_transform_and_scale(Plane &plane, const Vec3f &transform, const Vec3f rotate, const Vec2f &dimensions);
