#include <SDL2/SDL.h>
#include <cmath>


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

	Vec3f operator*(const Vec3f &b)
	{
		return Vec3f {
			x * b.x,
			y * b.y,
			z * b.z
		};
	}


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


#define FRUSTUM_VIEWPOINT_DISTANCE 0.001


/**
 * Get the squared distance between a vector and zero
 * @param v single vector
 */
float distance_squared(const Vec3f v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}


float distance_squared(const Vec3f &a, const Vec3f &b)
{
	return (a.x - b.z) * (a.x - b.x)
		+ (a.y - b.y) * (a.y - b.y)
		+ (a.z - b.z) * (a.z - b.z);
}

Vec2f ident(const Vec3f t, const char type)
{
	Vec2f v;
	switch(abs(type)) {
	case 1:
		v.x = t.x;
		v.y = t.y;
		break;
	case 2:
		v.x = t.y;
		v.y = t.z;
		break;
	case 3:
		v.x = t.x;
		v.y = t.z;
		break;
	}
	if(type < 0) {
		std::swap(v.x, v.y);
	}
	return v;
}

float line(Vec2f &coeff, float x)
{
	if(coeff.x == INFINITY || coeff.x == -INFINITY) {
		//If the slope is infinity, use the y-intercept as the only value
		return coeff.y;
	}
	return (x - coeff.y) / coeff.x;
}

bool check_infinity(Vec2f &coeffs, const float target)
{
	//If slope is infinite / vertical, set y-intercept to use same var the whole time
	if(coeffs.x == INFINITY || coeffs.x == -INFINITY) {
		coeffs.y = target;
		return true;
	}
	return false;
}


void coeffs(const Vec2f &a, const Vec2f &b, Vec2f &dest)
{
	//Slope is rise over run
	dest.x = (a.y - b.y) / (a.x - b.x);
	if(!check_infinity(dest, a.x)) {
		//Y-intercept is y - slope * x
		dest.y = a.y - (dest.x * a.x);
	}
}

void transform(Plane &plane, const Vec3f &translate, const Vec3f &rotate)
{
	Vec3f cosine = Vec3f { cos(rotate.x), cos(rotate.y), 0};
	Vec3f sine = Vec3f { sin(rotate.x), sin(rotate.y), 0 };
	for(u_char p=0; p<N_POINTS; p++) {
		plane.buffer[p] = plane.points[p] + translate;
		float x = (plane.buffer[p].x * cosine.y) - (plane.buffer[p].z * sine.y);
		float z = (plane.buffer[p].z * cosine.y) + (plane.buffer[p].x * sine.y);
		
		float y = (plane.buffer[p].y * cosine.x) - (z * sine.x);
		z = (z * cosine.x) + (plane.buffer[p].y * sine.x);
		plane.buffer[p].x = x;
		plane.buffer[p].y = y;
		plane.buffer[p].z = z;
	}
}

/**
 * @param plane container with applicable data to store 3d points in space and their respective 2d points
 * @param transform tranforms that move the camera
 * @param dimensions
 * The 2d points will be projected and accompanied by the z-distance of the transform
*/
void project_and_scale(Plane &plane, const Vec2f &dimensions)
{
	const float ratio = (dimensions.x / dimensions.y) * 500;
	for(uint p=0; p<N_POINTS; p++) {
		float z = plane.buffer[p].z;
		if(z < FRUSTUM_VIEWPOINT_DISTANCE) {
			z = FRUSTUM_VIEWPOINT_DISTANCE;
		}

		plane.buffer[p].x = ((plane.buffer[p].x
			* (1 / (z + FRUSTUM_VIEWPOINT_DISTANCE))) + 0.5) * dimensions.x;
		
		plane.buffer[p].y = ((plane.buffer[p].y 
			* (1 / (z + FRUSTUM_VIEWPOINT_DISTANCE))) + 0.5) * dimensions.y;
	}
}
