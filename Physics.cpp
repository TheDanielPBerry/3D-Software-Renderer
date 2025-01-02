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

Box *insert_box(Box *root, Box &box, uint dim)
{
	if(root == nullptr) {
		Box *b = &box;
		b->max = box.pos + box.dim;
		b->min = box.pos;
		b->left = nullptr;
		b->right = nullptr;
		return b;
	}
	 
	if(root->pos[dim] < box.pos[dim]) {
		root->right = insert_box(root->right, box, (dim+1)%3);
		root->max = max(root->max, root->right->max);
		root->min = min(root->min, root->right->min);
	} else {
		root->left = insert_box(root->left, box, (dim+1)%3);
		root->max = max(root->max, root->left->max);
		root->min = min(root->min, root->left->min);
	}
	return root;
}

uint number_of_checks = 0;
Box *intersects_tree(Box *root, Box &box, uint dim)
{
	if(root == nullptr) {
		return nullptr;
	}
	Box *n = nullptr;
	Box *b = nullptr;
	if(intersects(root, &box)) {
		b = root;
	}
	if(root->left != nullptr && ((root->left->max[dim] >= box.pos[dim]) && (root->left->min[dim] <= box.max[dim]))) {
		n = intersects_tree(root->left, box, (dim+1)%3);
		if(n!=nullptr) b = n;
	}
	if(root->right != nullptr && ((root->right->max[dim] >= box.pos[dim]) && (root->right->min[dim] <= box.max[dim]))) {
		n = intersects_tree(root->right, box, (dim+1)%3);
		if(n!=nullptr) b = n;
	}
	return b;
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
			box.max = box.pos + box.dim;
			float tempCoordinate;

			if(velocity.y != 0.0) {
				entity.grounded = false;
				tempCoordinate = box.pos.y;
				box.pos.y += velocity.y;
				box.max.y += velocity.y;
				Box *target = intersects_tree(staticTree, box, 0);
				if(target == nullptr) {
					target = intersects_entities(box, entities);
				}
				if(target != nullptr) {
					if(velocity.y > 0) {
						entity.grounded = true;
					}
					velocity.y = 0.0;
				}
				box.pos.y = tempCoordinate + velocity.y;
				box.max.y += velocity.y;
			}

			if(velocity.x != 0.0) {
				tempCoordinate = box.pos.x;
				box.pos.x += velocity.x;
				box.max.x += velocity.x;
				Box *target = intersects_tree(staticTree, box, 0);
				if(target == nullptr) {
					target = intersects_entities(box, entities);
				}
				if(target != nullptr) {
					if(entity.type == Player && entity.grounded) {
						box.pos.y -= 0.3;
						box.max.y -= 0.3;
						Box *diagonal = intersects_tree(staticTree, box, 0); 
						if(diagonal == nullptr) {
							diagonal = intersects_entities(box, entities);
						}
						if(diagonal == nullptr) {
							velocity.y -= 0.3;
							velocity.x *= 0.1;
							entity.grounded = false;
						} else {
							box.pos.y += 0.3;
							box.max.y += 0.3;
							velocity.x = 0;
						}
					} else {
						velocity.x = 0;
					}
				}
				box.pos.x = tempCoordinate;
				box.pos.x += velocity.x;
				box.max.x += velocity.x;
			}


			if(velocity.z != 0.0) {
				tempCoordinate = box.pos.z;
				box.pos.z += velocity.z;
				box.max.z += velocity.z;
				Box *target = intersects_tree(staticTree, box, 0);
				if(target == nullptr) {
					target = intersects_entities(box, entities);
				}
				if(target != nullptr) {
					if(entity.type == Player && entity.grounded) {
						box.pos.y -= 0.3;
						box.max.y -= 0.3;
						Box *diagonal = intersects_tree(staticTree, box, 0); 
						if(diagonal == nullptr) {
							diagonal = intersects_entities(box, entities);
						}
						if(diagonal == nullptr) {
							velocity.y -= 0.3;
							velocity.z *= 0.1;
							entity.grounded = false;
						} else {
							std::cout << velocity.y << std::endl;
							velocity.z = 0;
						}
					} else {
						velocity.z = 0;
					}
				}
			}
		}


		entity.pos = entity.pos + velocity;
		entity.vel = velocity / proportion;
		entity.rotation = (entity.rotation + entity.rotational_velocity * proportion);

		setRotationMatrix(entity, false);

	}
	number_of_checks = 0;
}
