#include <SDL2/SDL.h>
#include <cmath>

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

	Vec3f operator*(const Vec3f &b)
	{
		return Vec3f {
			x * b.x,
			y * b.y,
			z * b.z
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


#define FRUSTUM_VIEWPOINT_DISTANCE 0.01


/**
 * Get the squared distance between a vector and zero
 * @param v single vector
 */
float distance_squared(const Vec3f v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_transform_and_scale(Plane &plane, const Vec3f &transform, const Vec3f rotate, const Vec2f &dimensions)
{
	for(uint p=0; p<N_POINTS; p++) {
		float z = plane.points[p].z + transform.z + FRUSTUM_VIEWPOINT_DISTANCE;
		if(z < FRUSTUM_VIEWPOINT_DISTANCE) {
			z = FRUSTUM_VIEWPOINT_DISTANCE;
		}
		plane.buffer[p].x = (((plane.points[p].x + transform.x)
			* (1 / z)) + 0.5) * dimensions.y;

		plane.buffer[p].y = (((plane.points[p].y + transform.y)
			* (1 / z)) + 0.5) * dimensions.y;
		//Buffer Z contains squared distance from viewpoint to 2d pixel
		plane.buffer[p].z = distance_squared(plane.points[p] + transform);
	}
}
