
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>


//Each plane can be tested in the frustum
//
//Optimizations
//
//Frustum
//Intersecting and greater than planes go right
//Less than planes go left
//
//Back Face Culling
//If the order of points show a middle point on the wrong side of the top-bottom segment, then it shouldn't be drawn
#define P_SIZE 3
struct Plane {
	float points[P_SIZE][3];
	float color[4];
	//float average[P_SIZE];
	//float distance_squared;
	//float tx_coords[P_SIZE][2];
	//struct Plane *left;
	//struct Plane *right;
};

void project(Plane (*plane), float (*triangle), float (*transform))
{
	for(uint p=0; p<P_SIZE; p++) {
		const uint P_OFFSET = p * 8;
		triangle[P_OFFSET] = ((plane->points[p][0] + transform[0]) * (1 / ((plane->points[p][2] + transform[2]) + 0.01))) + 0.5;
		triangle[P_OFFSET + 1] = ((plane->points[p][1] + transform[1]) * (1 / ((plane->points[p][2] + transform[2]) + 0.01)) + 0.5);
		triangle[P_OFFSET + 2] = plane->color[0];
		triangle[P_OFFSET + 3] = plane->color[1];
		triangle[P_OFFSET + 4] = plane->color[2];
		triangle[P_OFFSET + 5] = plane->color[3];
	}
}


Plane *insert(Plane *root, Plane *p, uint dim)
{
	if(root == nullptr) {
		return p;
	}
	if(root == p) {
		return root;
	}
	return root;
}




void build_cube(std::vector<Plane> &chunk)
{
	//Top Face
	chunk.push_back(Plane{
		.points = {
			{0.5, -0.0, 3.5},
			{0.5, -0.0, 4.5},
			{-0.5, -0.0, 4.5},
		},
		.color ={1.0, 1.0, 0.0, 1.0}
	});
	chunk.push_back(Plane{
		.points = {
			{0.5, -0.0, 3.5},
			{-0.5, -0.0, 3.5},
			{-0.5, -0.0, 4.5},
		},
		.color = {1.0, 0.0, 1.0, 0.5}
	});

	//Bottom Face
	chunk.push_back(Plane{
		.points = {
			{0.5, 1.0, 3.5},
			{-0.5, 1.0, 3.5},
			{-0.5, 1.0, 4.5},
		},
		.color ={1.0, 1.0, 0.0, 1.0}
	});
	chunk.push_back(Plane{
		.points = {
			{0.5, 1.0, 3.5},
			{0.5, 1.0, 4.5},
			{-0.5, 1.0, 4.5},
		},
		.color = {1.0, 0.0, 1.0, 0.5}
	});

	//Left Face
	chunk.push_back(Plane{
		.points = {
			{-0.5, 1.0, 3.5},
			{-0.5, 1.0, 4.5},
			{-0.5, 0.0, 4.5},
		},
		.color ={1.0, 1.0, 0.0, 1.0}
	});
	chunk.push_back(Plane{
		.points = {
			{-0.5, 1.0, 3.5},
			{-0.5, 0.0, 3.5},
			{-0.5, 0.0, 4.5},
		},
		.color = {1.0, 0.0, 1.0, 0.5}
	});
}
