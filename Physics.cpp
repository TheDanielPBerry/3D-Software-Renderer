#include <vector>
#include <cmath>

#include "Physics.h"

/*
typedef struct Entity {
	Vec3f pos;
	Vec3f vel;
} Entity;


typedef struct Box {
	Vec3f pos;
	Vec3f dim;
	Entity *entity;
} Box;
*/

bool intersects(Box *a, Box *b)
{
	return a->pos.x <= b->pos.x + a->dim.x
	&& a->pos.x + a->dim.x >= b->pos.x
	&& a->pos.y <= b->pos.y + a->dim.y
	&& a->pos.y + a->dim.y >= b->pos.y
	&& a->pos.z <= b->pos.z + a->dim.z
	&& a->pos.z + a->dim.z >= b->pos.z;
}

Box *insert_box(Box *root, Box &box)
{
	if(root == nullptr) {
		Box *b = &box;
		return b;
	}

	uint branch = 0;
	for(unsigned char dim=0; dim<3; dim++) {
		if(box.pos[dim] + box.dim[dim] < root->pos[dim]) {
			branch += pow(2, dim);
		}
	}
	root->branches[branch] = insert_box(root->branches[branch], box);

	return root;
}

Box *intersects_tree(Box *root, Box *box)
{
	if(root == nullptr || intersects(root, box)) {
		return root;
	}
	uint branch = 0;
	for(unsigned char dim=0; dim<3; dim++) {
		if(box->pos[dim] + box->dim[dim] < root->pos[dim]) {
			branch += pow(2, dim);
		}
	}
	return intersects_tree(root->branches[branch], box);
}

void tick(std::vector<Entity> entities)
{
	for(auto entity : entities) {
		entity.pos = entity.pos + entity.vel;
	}
}
