#include <iostream>

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

void sort_by_axis(triangle2f t, uint axis)
{
	for(uint i=0; i<AXIS_SIZE; i++) {
		for(uint i=0; i<AXIS_SIZE; i++) {
		}
	}
}

void calculate_coeffecients(vec2f *a, vec2f *b, float (*coeff)[2])
{
	//Calculate Slope
	(*coeff)[0] = (a->axes[Y_AXIS] - b->axes[Y_AXIS]) /  (a->axes[X_AXIS] - b->axes[X_AXIS]);
	//Calculate y-intercept y = mx + b
	// b = y - mx
	(*coeff)[1] = a->axes[Y_AXIS] - (*coeff[0] * a->axes[X_AXIS]);
}
/**
* Get the slope and y-intercept for each pair of points in the triangle
* @re
*/
void calculate_coeffecients(triangle2f *t, float (*coeff)[3][2])
{
	for(uint p=0; p<TRIANGLE_SIZE; p++) {
		//Calculate slope = rise over run
		*coeff[p][0] = (t->points[p].axes[Y_AXIS] - t->points[p+1].axes[Y_AXIS]) /  (t->points[p].axes[X_AXIS] - t->points[p+1].axes[X_AXIS]);
		//Calculate y-intercept y = mx + b
		// b = y - mx
		*coeff[p][1] = t->points[p].axes[Y_AXIS] - (*coeff[p][0] * t->points[p].axes[X_AXIS]);
	}
}
}

