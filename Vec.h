#ifndef VEC_H
#define VEC_H

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

	Vec3f operator+(const Vec3f &b)
	{
		return Vec3f {
			x + b.x,
			y + b.y,
			z + b.z
		};
	}
	
	Vec3f operator-(const Vec3f &b)
	{
		return Vec3f {
			x - b.x,
			y - b.y,
			z - b.z
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

Vec2f ident(const Vec3f t, const char type);

Vec4f vec4f(Vec3f &v, float w);

//Return a vector that has components dimensions max or min of the 2
Vec3f max(Vec3f a, Vec3f b);
Vec3f min(Vec3f a, Vec3f b);

float line(Vec2f &coeff, float x);

void check_infinity(Vec2f &coeffs, const float target);

void coeffs(const Vec2f &a, const Vec2f &b, Vec2f &dest);

Vec3f cross_product(const Vec3f a, const Vec3f& b);
float dot_product(const Vec3f v, const Vec3f &n);

float distance_squared(const Vec3f &a, const Vec3f &b);
float distance_cubed(const Vec3f &a, const Vec3f &b);

void rotate(Vec3f &v, Vec3f (&trig)[2]);

Vec3f normalize(Vec3f v);

#endif
