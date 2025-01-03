#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <string>

#include "Vec.h"


typedef struct Box {
	Vec3f pos;
	Vec3f dim;
	Box *left;
	Box *right;
	Vec3f max;
	Vec3f min;
	struct Entity *entity;
} Box;

typedef enum EntityType {
	Player = 0,
	Object = 1,
	Rpg = 2,
	Shotgun = 3,
} EntityType;

typedef struct Entity {
	Vec3f pos;
	Vec3f vel;
	Vec3f drag;
	Vec3f rotation;
	Vec3f rotational_velocity;
	std::vector<Box> boxes;
	Vec3f rotationMatrix[2];
	bool grounded;
	enum EntityType type;
} Entity;

Box *insert_box(Box *root, Box &box, uint dim);

void setRotationMatrix(Entity &entity, bool initialize);
void tick(std::vector<Entity> &entities, Box *staticTree, uint milliseconds);

#endif
