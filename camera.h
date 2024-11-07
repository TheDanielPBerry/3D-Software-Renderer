#include <SDL2/SDL.h>
#include <vector>


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
void project(Plane (*plane), float (*triangle), float (*transform));
void build_cube(std::vector<Plane> &chunk);
