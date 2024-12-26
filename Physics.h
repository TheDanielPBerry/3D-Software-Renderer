#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "Vec.h"


typedef struct Box {
	Vec3f pos;
	Vec3f dim;
	Box *branches[8];
} Box;

typedef struct Entity {
	Vec3f pos;
	Vec3f vel;
	Vec3f rotation;
	Vec3f rotational_velocity;
	std::vector<Box> boxes;
} Entity;



#endif
