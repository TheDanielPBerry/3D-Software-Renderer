#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "geo/geo2d.h"


struct Buffer {
	uint dimensions[2];
	Uint32 (*pixels);
};

std::vector<geo2d::triangle2f> triangles;


struct Triangle2f {
	float points[3][2];
	Uint32 colors[3];
	float texture_coords[3][2];
};
/**
* First 3 floats are a, then 3 more for color of a
* [a.x, a.y, a.r, a.g, a.b, a.tx, a.ty] 
* [b.x, b.y, b.r, b.g, b.b, b.tx, b.ty] 
* [c.x, c.y, c.r, c.g, c.b, c.tx, c.ty] 
*/
void fill_triangles(Buffer *buffer, const float (*data), uint NUM_TRIANGLES)
{
	const uint NUM_POINTS = 3;

	const uint VEC_X = 0;
	const uint VEC_Y = 1;
	const uint VEC_RED = 2;
	const uint VEC_GREEN = 3;
	const uint VEC_BLUE = 4;
	const uint VEC_ALPHA = 5;
	const uint VEC_TEX_X = 5;
	const uint VEC_TEX_Y = 7;
	const uint P_SIZE = 8;

	Triangle2f geometry;
	Triangle2f *triangle = &geometry;

	uint next_triangle = (NUM_POINTS * P_SIZE);
	for(uint t=0; t<NUM_TRIANGLES; t++) {
		uint t_offset = t * P_SIZE;
		for(uint p=0; p<NUM_POINTS; p++) {
			const uint P_OFFSET = t_offset + (p * P_SIZE);
			triangle->points[p][0] = data[P_OFFSET+VEC_X] * buffer->dimensions[0];
			triangle->points[p][1] = data[P_OFFSET+VEC_Y] * buffer->dimensions[1];

			unsigned int color = ((uint)(data[P_OFFSET+VEC_RED] * 255)) << 24;
			color += ((uint)(data[P_OFFSET+VEC_GREEN] * 255)) << 16;
			color += ((uint)(data[P_OFFSET+VEC_BLUE] * 255)) << 8;
			color += ((uint)(data[P_OFFSET+VEC_ALPHA] * 255));
			triangle->colors[p] = color;

			triangle->texture_coords[p][0] = data[P_OFFSET+VEC_TEX_X];
			triangle->texture_coords[p][1] = data[P_OFFSET+VEC_TEX_Y];
		}
		//References to which of the points is which in y-order
		uint top = 0, middle = 1, bottom = 2;

		//Sort the three points from top to bottom on the y-axis
		uint temp;
		if(triangle->points[top][1] > triangle->points[middle][1]) {
			temp = top;
			top = middle;
			middle = temp;
		} 
		if(triangle->points[middle][1] > triangle->points[bottom][1]) {
			temp = bottom;
			bottom = middle;
			middle = temp;
		}
		if(triangle->points[top][1] > triangle->points[middle][1]) {
			temp = top;
			top = middle;
			middle = temp;
		}


		//Calculate coeffecients of first set of lines
		float left[2];
		float right[2];
		//If the bottom and middle are on the same side of the top
		bool leftMiddleOriented = (((triangle->points[bottom][0] - triangle->points[top][0]) 
			* (triangle->points[middle][1] - triangle->points[top][1]))
			- ((triangle->points[bottom][1] - triangle->points[top][1])  
			* (triangle->points[middle][0] - triangle->points[top][0]))) > 0;

		if(leftMiddleOriented) {
			left[0] = (triangle->points[top][1] - triangle->points[middle][1]) /  (triangle->points[top][0] - triangle->points[middle][0]);
			left[1] = triangle->points[top][1] - (left[0] * triangle->points[top][0]);

			right[0] = (triangle->points[top][1] - triangle->points[bottom][1]) /  (triangle->points[top][0] - triangle->points[bottom][0]);
			right[1] = triangle->points[top][1] - (right[0] * triangle->points[top][0]);
		} else {
			right[0] = (triangle->points[top][1] - triangle->points[middle][1]) /  (triangle->points[top][0] - triangle->points[middle][0]);
			right[1] = triangle->points[top][1] - (right[0] * triangle->points[top][0]);

			left[0] = (triangle->points[top][1] - triangle->points[bottom][1]) /  (triangle->points[top][0] - triangle->points[bottom][0]);
			left[1] = triangle->points[top][1] - (left[0] * triangle->points[top][0]);
		}

		Uint32 color = 0xFFFFFFFF;

		if(abs(left[0]) > 0.01 && abs(right[0]) > 0.01) {
			uint yStart = std::max((float)0.0, triangle->points[top][1]);
			uint yEnd = std::min(triangle->points[middle][1], (float)buffer->dimensions[1]);
			for(uint y=yStart; y<yEnd; y++) {
				int yOffset = y * buffer->dimensions[0];

				float xLeft = ((y - left[1]) / left[0]);
				if(std::isnan(xLeft)) {
					break;
				}
				xLeft = std::max((float)0.0, xLeft);
				xLeft = std::min(xLeft, (float)buffer->dimensions[0]);

				float xRight = (int)((y - right[1]) / right[0]);
				if(std::isnan(xRight)) {
					break;
				}
				xRight = std::max((float)0.0, xRight);
				xRight = std::min(xRight, (float)buffer->dimensions[0]);

				for(int x=(uint)xLeft; x<(uint)xRight; x++) {
					buffer->pixels[yOffset + x] = (buffer->dimensions[0] / (float)x) * 0xFFFFFFFF;
				}
			}
		}

		if(leftMiddleOriented) {
			left[0] = (triangle->points[bottom][1] - triangle->points[middle][1]) /  (triangle->points[bottom][0] - triangle->points[middle][0]);
			left[1] = triangle->points[bottom][1] - (left[0] * triangle->points[bottom][0]);
		} else {
			right[0] = (triangle->points[bottom][1] - triangle->points[middle][1]) /  (triangle->points[bottom][0] - triangle->points[middle][0]);
			right[1] = triangle->points[bottom][1] - (right[0] * triangle->points[bottom][0]);
		}

		if(abs(left[0]) > 0.01 && abs(right[0]) > 0.01) {
			uint yStart = std::max((float)0.0, triangle->points[middle][1]);
			uint yEnd = std::min(triangle->points[bottom][1], (float)buffer->dimensions[1]);
			for(uint y=yStart; y<yEnd; y++) {
				int yOffset = y * buffer->dimensions[0];

				float xLeft = ((y - left[1]) / left[0]);
				if(std::isnan(xLeft)) {
					break;
				}
				xLeft = std::max((float)0.0, xLeft);
				xLeft = std::min(xLeft, (float)buffer->dimensions[0]);

				float xRight = ((y - right[1]) / right[0]);
				if(std::isnan(xRight)) {
					break;
				}
				xRight = std::max((float)0.0, xRight);
				xRight = std::min(xRight, (float)buffer->dimensions[0]);

				for(uint x=(uint)xLeft; x<(uint)xRight; x++) {
					buffer->pixels[yOffset + x] = (buffer->dimensions[0] / (float)x) * 0xFFFFFFFF;
				}
			}
		}
	}
}


