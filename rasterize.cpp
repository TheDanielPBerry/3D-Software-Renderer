
#include "Plane.h"

/**
* The plan for this function is to take points that are already projected and mapped for a buffer and rasterize them.
* I think the z_buffer will need to be filled with zeros the same as the main buffer
*/
void rasterize(Plane &plane, Uint32 *buffer, Vec2f dimensions, float *z_buffer)
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
		temp = top;
		top = middle;
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
	bool leftOrientedTriangle = (((plane.points[bottom].x - plane.points[top].x) 
			* (plane.points[middle].y - plane.points[top].y))
			- ((plane.points[bottom].y - plane.points[top].y)  
			* (plane.points[middle].x - plane.points[top].x))) > 0;
	
	//X is slope and the Y is the y-intercept of each line
	Vec2f leftCoeff;
	Vec2f rightCoeff;
	if(leftOrientedTriangle) {
			leftCoeff.x = (plane.points[top].y - plane.points[middle].y) /  (plane.points[top].x - plane.points[middle].x);
			leftCoeff.y = plane.points[top].y - (leftCoeff.x * plane.points[top].x);

			rightCoeff.x = (plane.points[top].y - plane.points[bottom].y) /  (plane.points[top].x - plane.points[bottom].x);
			rightCoeff.y = plane.points[top].y - (rightCoeff.x * plane.points[top].x);
	} else {
			leftCoeff.x = (plane.points[top].y - plane.points[bottom].y) /  (plane.points[top].x - plane.points[bottom].x);
			leftCoeff.y = plane.points[top].y - (leftCoeff.x * plane.points[top].x);

			rightCoeff.x = (plane.points[top].y - plane.points[middle].y) /  (plane.points[top].x - plane.points[middle].x);
			rightCoeff.y = plane.points[top].y - (rightCoeff.x * plane.points[top].x);
	}
	

}
