
namespace geo2d {
	#define AXIS_SIZE 2
	#define X_AXIS 0
	#define Y_AXIS 1

	#define TRIANGLE_SIZE 3

	typedef struct vec2f {
		float axes[AXIS_SIZE];
	} vec2f;

	typedef struct triangle2f {
			vec2f points[TRIANGLE_SIZE];
	} triangle2f;


	void calculate_coeffecients(vec2f *a, vec2f *b, float (*coeff)[2]);
	void calculate_coeffecients(triangle2f *t, float (*coeff)[3][2]);
}
