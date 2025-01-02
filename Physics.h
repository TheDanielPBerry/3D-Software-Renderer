#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <string>

#include "Vec.h"


typedef struct Box {
	Vec3f src;
	Vec3f dest;
	Box *left;
	Box *right;
	struct Entity *entity;
} Box;

typedef struct Entity {
	Vec3f pos;
	Vec3f vel;
	Vec3f drag;
	Vec3f rotation;
	Vec3f rotational_velocity;
	std::vector<Box> boxes;
	Vec3f rotationMatrix[2];
	bool grounded;
	std::string name;
} Entity;

Box *insert_box(Box *root, Box &box, uint dim);

void setRotationMatrix(Entity &entity, bool initialize);
void tick(std::vector<Entity> &entities, Box *staticTree, uint milliseconds);

#endif
