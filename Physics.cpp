#include <vector>
#include <iostream>
#include <cmath>
#include "Physics.h"


bool intersects(Box *a, Box *b)
{
	return a->pos.x <= b->pos.x + b->dim.x
	&& a->pos.x + a->dim.x >= b->pos.x
	&& a->pos.y <= b->pos.y + b->dim.y
	&& a->pos.y + a->dim.y >= b->pos.y
	&& a->pos.z <= b->pos.z + b->dim.z
	&& a->pos.z + a->dim.z >= b->pos.z;
}

Box *insert_box(Box *root, Box &box)
{
	if(root == nullptr) {
		Box *b = &box;
		for(unsigned char branches=0; branches<8; branches++) {
			b->branches[branches] = nullptr;
		}
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

uint number_of_checks = 0;
Box *intersects_tree(Box *root, Box box)
{
	number_of_checks++;
	if(root == nullptr || intersects(root, &box)) {
		return root;
	}
	uint branch = 0;
	for(unsigned char dim=0; dim<3; dim++) {
		if(box.pos[dim] + box.dim[dim] < root->pos[dim]) {
			branch += pow(2, dim);
		}
	}
	return intersects_tree(root->branches[branch], box);
}

void tick(std::vector<Entity> &entities, Box *staticTree)
{
	for(auto &entity : entities) {
		Vec3f velocity = entity.vel;
		velocity.y += 0.1;
		velocity = velocity * Vec3f{0.9, 0.9, 0.9};

		//Check each axis individually
		//x axis
		for(auto box : entity.boxes) {
			box.pos = box.pos + entity.pos;
			float tempCoordinate;

			if(velocity.x != 0.0) {
				tempCoordinate = box.pos.x;
				box.pos.x += velocity.x;
				if(intersects_tree(staticTree, box) != nullptr) {
					velocity.x = 0.0;
				}
				box.pos.x = tempCoordinate;
			}

			if(velocity.y != 0.0) {
				number_of_checks = 0;
				tempCoordinate = box.pos.y;
				box.pos.y += velocity.y;
				if(intersects_tree(staticTree, box) != nullptr) {
					velocity.y = 0.0;
				}
				box.pos.y = tempCoordinate;
				std::cout << number_of_checks << std::endl;
			}

			if(velocity.z != 0.0) {
				tempCoordinate = box.pos.z;
				box.pos.z += velocity.z;
				if(intersects_tree(staticTree, box) != nullptr) {
					velocity.z = 0.0;
				}
				box.pos.z = tempCoordinate;
			}
		}


		entity.vel = velocity;
		entity.pos = entity.pos + velocity;
		entity.rotation = entity.rotation + entity.rotational_velocity;

		entity.rotationMatrix[0] = Vec3f{ float(cos(entity.rotation.x)), float(cos(entity.rotation.y)), float(cos(entity.rotation.z)) };
		entity.rotationMatrix[1] = Vec3f{ float(sin(entity.rotation.x)), float(sin(entity.rotation.y)), float(sin(entity.rotation.z)) };


	}
}
