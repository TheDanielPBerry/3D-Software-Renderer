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
		b->max = box.pos + box.dim;
		for(unsigned char branches=0; branches<8; branches++) {
			b->branches[branches] = nullptr;
		}
		return b;
	}

	uint branch = 0;
	for(unsigned char dim=0; dim<3; dim++) {
		if(box.pos[dim] > root->pos[dim]) {
			branch += pow(2, dim);
		}
	}
	root->branches[branch] = insert_box(root->branches[branch], box);
	root->max = max(root->max, root->branches[branch]->max);

	return root;
}

uint number_of_checks = 0;
Box *intersects_tree(Box *root, Box box)
{
	if(root == nullptr || intersects(root, &box)) {
		return root;
	}
	uint branch = 0;
	Box *left;
	for(unsigned char dim=0; dim<3; dim++) {
		left = root->branches[branch + (dim*2)];
		if(left == nullptr || box.pos[dim] > left->max[dim]) {
			branch += pow(2, dim);
		}
	}
	return intersects_tree(root->branches[branch], box);
}

void setRotationMatrix(Entity &entity, bool initialize)
{
	for(uint d=0; d<3; d++) {
		if(entity.rotational_velocity[d] != 0.0 || initialize) {
			entity.rotationMatrix[0][d] = cos(entity.rotation[d]);
			entity.rotationMatrix[1][d] = sin(entity.rotation[d]);
		}
	}
}

Box *intersects_entities(Box &box, std::vector<Entity> &entities)
{
	Box *b = nullptr;
	for(Entity &entity : entities) {
		if(box.entity != &entity) {
			for(Box &target : entity.boxes) {
				target.pos = target.pos + entity.pos;
				//number_of_checks++;
				if(intersects(&target, &box)) {
					b = &target;
				}
				target.pos = target.pos - entity.pos;
			}
		}
	}
	return b;
}

void tick(std::vector<Entity> &entities, Box *staticTree, uint milliseconds)
{
	const float proportion = milliseconds / 1000.0;
	//const float proportion = 1;
	for(auto &entity : entities) {
		//Forces
		Vec3f velocity = entity.vel;
		velocity.y += (180.0 * proportion);	//Gravity
		velocity = velocity * (entity.drag);


		velocity = velocity * proportion;
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
				} else {
					Box *target = intersects_entities(box, entities);
					if(target != nullptr) {
						velocity.x = 0;
					}
				}
				box.pos.x = tempCoordinate;
				box.pos.x += velocity.x;
			}

			if(velocity.y != 0.0) {
				entity.grounded = false;
				tempCoordinate = box.pos.y;
				box.pos.y += velocity.y;
				if(intersects_tree(staticTree, box) != nullptr) {
					if(velocity.y > 0) {
						entity.grounded = true;
					}
					velocity.y = 0.0;
				} else {
					Box *target = intersects_entities(box, entities);
					if(target != nullptr) {
						if(velocity.y > 0) {
							entity.grounded = true;
						}
						velocity.y = 0;
					}
				}
				box.pos.y = tempCoordinate;
				box.pos.y += velocity.y;
			}

			if(velocity.z != 0.0) {
				tempCoordinate = box.pos.z;
				box.pos.z += velocity.z;
				if(intersects_tree(staticTree, box) != nullptr) {
					velocity.z = 0.0;
				} else {
					Box *target = intersects_entities(box, entities);
					if(target != nullptr) {
						velocity.z = 0;
					}
				}
				box.pos.z = tempCoordinate;
				box.pos.z += velocity.z;
			}
		}


		entity.pos = entity.pos + velocity;
		entity.vel = velocity / proportion;
		entity.rotation = (entity.rotation + entity.rotational_velocity * proportion);

		setRotationMatrix(entity, false);

	}
	//std::cout << number_of_checks << std::endl;
	number_of_checks = 0;
}
