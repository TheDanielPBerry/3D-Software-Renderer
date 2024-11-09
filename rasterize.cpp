
#include <vector>
#include <cmath>

#include "Plane.h"



void interpolate_lines(
	Plane &plane,
	const Vec2f &left,
	const Vec2f &right,
	Vec2f &y_bounds,
	const Vec2f &z_bounds_left,
	const Vec2f &z_bounds_right,
	const Vec2f &dimensions,
	Uint32 *buffer,
	float *z_buffer
)
{
	if(abs(left.x) < 0.01 || abs(right.x) < 0.01) {
		//If either of the slopes are near horizontal, we don't need to draw it
		return;
	}

	y_bounds.x = std::min(y_bounds.x, dimensions.y);
	y_bounds.y = std::max((float)0.0, y_bounds.y);

	//Set the left and right bounds to the second value on the function
	//If the slope is infinite it will use this for the entire range
	float xLeft = left.y, xRight = right.y;

	//Used to calculate progress from top to bottom
	float vertical_diff = y_bounds.y - y_bounds.x;
	float left_slope = (z_bounds_left.y - z_bounds_left.x) / vertical_diff;
	float right_slope = (z_bounds_right.y - z_bounds_right.x) / vertical_diff;
	float left_intercept = z_bounds_left.x - (left_slope * y_bounds.x);
	float right_intercept = z_bounds_right.x - (right_slope * y_bounds.x);

	Vec2f middle = Vec2f {
		dimensions.x / 2,
		dimensions.y / 2,
	};
	
	uint alpha = plane.color % 256;
	plane.color /= 256;
	uint blue = plane.color % 256;
	plane.color /= 256;
	uint green = plane.color % 256;
	plane.color /= 256;
	uint red = plane.color % 256;

	for(uint y=y_bounds.x; y<y_bounds.y; y++) {
		uint yOffset = y * dimensions.x;

		if(left.x != INFINITY && left.x != -INFINITY) {
			xLeft = ((y - left.y) / left.x);
		}
		if(std::isnan(xLeft)) {
			return;
		}
		xLeft = std::max((float)0.0, xLeft);
		xLeft = std::min(xLeft, dimensions.x);

		if(right.x != INFINITY && right.x != -INFINITY) {
			xRight = ((y - right.y) / right.x);
		}
		if(std::isnan(xRight)) {
			return;
		}
		xRight = std::max((float)0.0, xRight);
		xRight = std::min(xRight, dimensions.x);

		float y_percentage = (y - y_bounds.x) / vertical_diff;
		float z_left = (left_slope * y) + left_intercept;
		float z_right = (right_slope * y) + right_intercept;
		//float z = (z_left + z_right) / 2.0;
		float z_slope = (z_right - z_left) / (xRight - xLeft);
		float z_intercept = z_left - (z_slope * xLeft);

		for(uint x=xLeft; x<xRight; x++) {
			float z = (z_slope * x) + z_intercept;
			if(z_buffer[yOffset + x] > z && z > 0.1) {
				float z_inverse = 10000/(z*z);
				float cone = 10000 / ((x - middle.x)*(x - middle.x) + (y-middle.y)*(y-middle.y));

				Uint32 pixel = (std::min((int)(red * z_inverse * cone),(int)red) << 24);
				pixel += (std::min((int)(green * z_inverse * cone), (int)green) << 16);
				pixel += (std::min((int)(blue * z_inverse * cone), (int)blue) << 8);
				pixel += alpha;
				buffer[yOffset + x] = pixel;
				z_buffer[yOffset + x] = z;
			}
		}
	}
}

/**
* The plan for this function is to take points that are already projected and mapped for a buffer and rasterize them.
* I think the z_buffer will need to be filled with zeros the same as the main buffer
*/
void rasterize(Plane &plane, Uint32 *buffer, const Vec2f &dimensions, float *z_buffer)
{
	char top = 0, middle = 1, bottom = 2;
	//Test if all the 2d points are under off of the buffer in one direction or the other
	if(plane.buffer[top].x < 0.0 && plane.buffer[middle].x < 0.0 && plane.buffer[bottom].x < 0.0) {
		return;
	}
	if(plane.buffer[top].x > dimensions.x && plane.buffer[middle].x > dimensions.x && plane.buffer[bottom].x > dimensions.x) {
		return;
	}
	if(plane.buffer[top].y < 0.0 && plane.buffer[middle].y < 0.0 && plane.buffer[bottom].y < 0.0) {
		return;
	}
	if(plane.buffer[top].y > dimensions.y && plane.buffer[middle].y > dimensions.y && plane.buffer[bottom].y > dimensions.y) {
		return;
	}


	char temp;
	//Sort the points from top to bottom on the y axis
	if(plane.buffer[top].y > plane.buffer[middle].y) {
		temp = top;
		top = middle;
		middle = temp;
	}
	if(plane.buffer[middle].y > plane.buffer[bottom].y) {
		temp = bottom;
		bottom = middle;
		middle = temp;
	}
	if(plane.buffer[top].y > plane.buffer[middle].y) {
		temp = top;
		top = middle;
		middle = temp;
	}

	//Calculate coefficients for left and right lines
	//If the middle point is on one side of the line or the other will determine if the triangle is left or right oriented
	//Can calculate the side using the dot product of the 3 points in 2d
	bool leftOrientedTriangle = (((plane.buffer[bottom].x - plane.buffer[top].x) 
			* (plane.buffer[middle].y - plane.buffer[top].y))
			- ((plane.buffer[bottom].y - plane.buffer[top].y)
			* (plane.buffer[middle].x - plane.buffer[top].x))) > 0;
	
	//X is slope and the Y is the y-intercept of each line
	Vec2f leftCoeff;
	Vec2f rightCoeff;

	Vec2f z_bounds_left;
	Vec2f z_bounds_right;
	if(leftOrientedTriangle) {
		leftCoeff.x = (plane.buffer[top].y - plane.buffer[middle].y) / (plane.buffer[top].x - plane.buffer[middle].x);
		leftCoeff.y = plane.buffer[top].y - (leftCoeff.x * plane.buffer[top].x);
		z_bounds_left = Vec2f{plane.buffer[top].z, plane.buffer[middle].z};

		rightCoeff.x = (plane.buffer[top].y - plane.buffer[bottom].y) / (plane.buffer[top].x - plane.buffer[bottom].x);
		rightCoeff.y = plane.buffer[top].y - (rightCoeff.x * plane.buffer[top].x);
		z_bounds_right = Vec2f{plane.buffer[top].z, plane.buffer[bottom].z};
	} else {
		leftCoeff.x = (plane.buffer[top].y - plane.buffer[bottom].y) / (plane.buffer[top].x - plane.buffer[bottom].x);
		leftCoeff.y = plane.buffer[top].y - (leftCoeff.x * plane.buffer[top].x);
		z_bounds_left = Vec2f{plane.buffer[top].z, plane.buffer[bottom].z};

		rightCoeff.x = (plane.buffer[top].y - plane.buffer[middle].y) / (plane.buffer[top].x - plane.buffer[middle].x);
		rightCoeff.y = plane.buffer[top].y - (rightCoeff.x * plane.buffer[top].x);
		z_bounds_right = Vec2f{plane.buffer[top].z, plane.buffer[middle].z};
	}
	if(leftCoeff.x == INFINITY || leftCoeff.x == -INFINITY) {
		leftCoeff.y = plane.buffer[top].x;
	}
	if(rightCoeff.x == INFINITY || rightCoeff.x == -INFINITY) {
		rightCoeff.y = plane.buffer[top].x;
	}

	Vec2f y_bounds = Vec2f {
		std::max(plane.buffer[top].y, (float)0.0),
		std::min(plane.buffer[middle].y, dimensions.y)
	};


	interpolate_lines(plane, leftCoeff, rightCoeff, y_bounds, z_bounds_left, z_bounds_right, dimensions, buffer, z_buffer);

	if(leftOrientedTriangle) {
		leftCoeff.x = (plane.buffer[middle].y - plane.buffer[bottom].y) / (plane.buffer[middle].x - plane.buffer[bottom].x);
		leftCoeff.y = plane.buffer[bottom].y - (leftCoeff.x * plane.buffer[bottom].x);
		if(leftCoeff.x == INFINITY || leftCoeff.x == -INFINITY) {
			leftCoeff.y = plane.buffer[bottom].x;
		} else {
			leftCoeff.y = plane.buffer[bottom].y - (leftCoeff.x * plane.buffer[bottom].x);
		}
		z_bounds_left = Vec2f{plane.buffer[middle].z, plane.buffer[bottom].z};
	} else {
		rightCoeff.x = (plane.buffer[middle].y - plane.buffer[bottom].y) / (plane.buffer[middle].x - plane.buffer[bottom].x);
		if(rightCoeff.x == INFINITY || rightCoeff.x == -INFINITY) {
			rightCoeff.y = plane.buffer[bottom].x;
		} else {
			rightCoeff.y = plane.buffer[bottom].y - (rightCoeff.x * plane.buffer[bottom].x);
		}
		z_bounds_right = Vec2f{plane.buffer[middle].z, plane.buffer[bottom].z};
	}
	y_bounds = Vec2f {
		std::max(plane.buffer[middle].y, (float)0.0),
		std::min(plane.buffer[bottom].y, dimensions.y)
	};
	interpolate_lines(plane, leftCoeff, rightCoeff, y_bounds, z_bounds_left, z_bounds_right, dimensions, buffer, z_buffer);
}

void draw_scene(std::vector<Plane> scene, Uint32 *buffer, const Vec2f &dimensions, const Vec3f &transform, const Vec3f &rotate, float *z_buffer)
{
	for(uint p=0; p<scene.size(); p++) {
		//Perhaps a nice way to cull rearward faces
		//If one point is behind the camera, make the z 0
		//If 2 points are behind, hide the triangle
		if((scene[p].points[0].z + transform.z) < 0 
			&& (scene[p].points[1].z + transform.z) < 0 
			&& (scene[p].points[2].z + transform.z) < 0) {
			//Don't bother with triangles behind the camera
			continue;
		}
		project_transform_and_scale(scene[p], transform, rotate, dimensions);
		rasterize(scene[p], buffer, dimensions, z_buffer);
	}
}
