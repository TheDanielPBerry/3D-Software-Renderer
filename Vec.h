
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

Vec2f ident(const Vec3f t, const char type);

float line(Vec2f &coeff, float x);

void check_infinity(Vec2f &coeffs, const float target);

void coeffs(const Vec2f &a, const Vec2f &b, Vec2f &dest);

float distance_squared(const Vec3f &a, const Vec3f &b);
float distance_cubed(const Vec3f &a, const Vec3f &b);

Vec3f cross_product(const Vec3f a, const Vec3f& b);
float dot_product(const Vec3f v, const Vec3f &n);
