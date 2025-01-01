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
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

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

	Vec3f operator*(const float &b)
	{
		return Vec3f {
			x * b,
			y * b,
			z * b
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

	Vec3f operator/(const float &b)
	{
		return Vec3f {
			x / b,
			y / b,
			z / b
		};
	}

	float &operator[](const int dim)
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

	Vec4f operator*(const Vec4f &b)
	{
		return Vec4f {
			x * b.x,
			y * b.y,
			z * b.z,
			w * b.w,
		};
	}
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

Vec4f vec4f(Vec3f &v, float w)
{
	return Vec4f {
		v.x,
		v.y,
		v.z,
		w,
	};
}

Vec3f max(Vec3f a, Vec3f b)
{
	return Vec3f {
		std::max(a.x, b.x),
		std::max(a.y, b.y),
		std::max(a.z, b.z),
	};
}
Vec3f min(Vec3f a, Vec3f b)
{
	return Vec3f {
		std::min(a.x, b.x),
		std::min(a.y, b.y),
		std::min(a.z, b.z),
	};
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
	return ((a.x - b.x) * (a.x - b.x))
		+ ((a.y - b.y) * (a.y - b.y))
		+ ((a.z - b.z) * (a.z - b.z));
}
float distance_cubed(const Vec3f &a, const Vec3f &b)
{
	return (a.x - b.z) * (a.x - b.x) * (a.x - b.x)
		+ (a.y - b.y) * (a.y - b.y) * (a.y - b.y)
		+ (a.z - b.z) * (a.z - b.z) * (a.z - b.z);
}

void rotate(Vec3f &v, Vec3f (&trig)[2])
{
	#define COSINE 0
	#define SINE 1
	float x, y, temp_y, z;
	//Y-axis
	x = (v.x * trig[COSINE].y) - (v.z * trig[SINE].y);
	z = (v.z * trig[COSINE].y) + (v.x * trig[SINE].y);

	//Then the x-axis
	temp_y = (v.y * trig[COSINE].x) - (z * trig[SINE].x);
	z = (z * trig[COSINE].x) + (v.y * trig[SINE].x);

	//Lastly the z-axis
	y = (temp_y * trig[COSINE].z) - (x * trig[SINE].z);
	x = (x * trig[COSINE].z) + (temp_y * trig[SINE].z);
	v = Vec3f{x, y, z};
}

Vec3f normalize(Vec3f v)
{
	float distance = sqrt(distance_squared(v));
	return v / distance;
}
