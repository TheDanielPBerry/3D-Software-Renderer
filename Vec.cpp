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

	Vec3f operator-(const Vec3f &b)
	{
		return Vec3f {
			x - b.x,
			y - b.y,
			z - b.z
		};
	}

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

	Vec3f operator/(const Vec3f &b)
	{
		return Vec3f {
			x / b.x,
			y / b.y,
			z / b.z
		};
	}

	float operator[](const int dim)
	{
		if(dim == 0) {
			return x;
		} else if(dim == 1) {
			return y;
		}
		return z;
	}
} Vec3f;

typedef struct Vec4f {
	float x;
	float y;
	float z;
	float w;
} Vec4f;

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

Vec3f cross_product(const Vec3f a, const Vec3f& b)
{
	return Vec3f{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float dot_product(const Vec3f v, const Vec3f &n)
{
	return (v.x*n.x) + (v.y*n.y) + (v.z*n.z);
}

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
float distance_cubed(const Vec3f &a, const Vec3f &b)
{
	return (a.x - b.z) * (a.x - b.x) * (a.x - b.x)
		+ (a.y - b.y) * (a.y - b.y) * (a.y - b.y)
		+ (a.z - b.z) * (a.z - b.z) * (a.z - b.z);
}
