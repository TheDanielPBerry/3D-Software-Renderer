
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
	float average[P_SIZE];
	float distance_squared;
	float tx_coords[P_SIZE][2];
	Uint32 color;
	struct Plane *left;
	struct Plane *right;
};

void project(Plane (*plane), float (*triangle))
{
	for(uint p=0; p<P_SIZE; p++) {
		triangle[p] = plane->points[p][0] * (1 / (plane->points[p][2] + 0.01));
		triangle[p+1] = plane->points[p][1] * (1 / (plane->points[p][2] + 0.01));
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



std::vector<Plane> chunk;

void build_cube()
{
	chunk.push_back(Plane{
		.points = {
			{0.75, 0.75, 5.0},
			{0.25, 0.75, 5.0},
			{0.25, 0.25, 5.0},
		},
		.color = 0xFF00FFFF
	});
	chunk.push_back(Plane{
		.points = {
			{0.75, 0.75, 5.0},
			{0.75, 0.25, 5.0},
			{0.25, 0.25, 5.0},
		},
		.color = 0xFF00FFFF
	});
}
