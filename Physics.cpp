#include <vector>
#include <iostream>
#include <cmath>
#include "Physics.h"



bool intersects(Box *a, Box *b)
{
	return a->src.x <= b->dest.x
	&& a->dest.x >= b->src.x
	&& a->src.y <= b->dest.y
	&& a->dest.y >= b->src.y
	&& a->src.z <= b->dest.z
	&& a->dest.z >= b->src.z;
}

Vec3f volume(Box &a, Box &b)
{
	return Vec3f {
		std::min(a.dest.x, b.dest.x)-std::max(a.src.x, b.src.x),
		std::min(a.dest.y, b.dest.y)-std::max(a.src.y, b.src.y),
		std::min(a.dest.z, b.dest.z)-std::max(a.src.z, b.src.z),
	};
}

Box *insert_box(Box *root, Box &box, uint dim)
{
	if(root == nullptr) {
		Box *b = &box;
		b->left = nullptr;
		b->right = nullptr;
		return b;
	}
	
	if(root->left == nullptr) {
		root->right = nullptr;
		Box *b = new Box;
		b->src = root->src;
		b->dest = root->dest;
		b->left = root;
		b->right = nullptr;
		root = b;
	}
	if(root->src[dim] < box.src[dim]) {
		root->right = insert_box(root->right, box, (dim+1)%3);
		root->src = min(root->right->src, root->src);
		root->dest = max(root->right->dest, root->dest);

	} else {
		root->left = insert_box(root->left, box, (dim+1)%3);
		root->src = min(root->left->src, root->src);
		root->dest = max(root->left->dest, root->dest);
	}
	return root;
}

void recalculate_box_ray(Box &b, Vec3f &vel)
{
}

uint number_of_checks = 0;
Vec3f intersects_tree(Box *root, Box &box, uint dim)
{
	Vec3f vol = Vec3f{-1,-1,-1};
	if(root == nullptr) {
		return vol;
	}
	number_of_checks++;

	Vec3f v = volume(*root, box);
	if(v[0] >= 0 && v[1] >= 0 && v[2] >= 0) {
		if(root->left == nullptr) {
			std::cout << v.x << " " << v.y << " " << v.z << "\n";
			return v;
		} else {
			v = intersects_tree(root->left, box, (dim+1)%3);
			if(v[0] > 0 && v[1] > 0 && v[2] > 0) {
				if(vol[dim] < 0) {
					vol = v;
				} else {
					vol = min(v, vol);
				}
			}
			if(root->right != nullptr) {
				v = intersects_tree(root->right, box, (dim+1)%3);
				if(v[0] > 0 && v[1] > 0 && v[2] > 0) {
					if(vol[dim] < 0) {
						vol = v;
					} else {
						vol = min(v, vol);
					}
				}
			}
		}
	}
	return vol;
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
				target.src = target.src + entity.pos;
				//number_of_checks++;
				if(intersects(&target, &box)) {
					b = &target;
				}
				target.src = target.src - entity.pos;
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
		//velocity.y += (180.0 * proportion);	//Gravity
		velocity = velocity * (entity.drag);


		velocity = velocity * proportion;
		//Check each axis individually
		//x axis
		for(auto box : entity.boxes) {
			box.src = box.src + entity.pos;
			box.dest = box.dest + entity.pos;
			float tempCoordinate;

			if(velocity.x != 0.0) {
				tempCoordinate = box.src.x;
				box.src.x += velocity.x;
				Vec3f volume = intersects_tree(staticTree, box, 0);
				if(volume.x > 0 && volume.y > 0 && volume.z > 0) {
					velocity.x = 0.0;
				} else {
					Box *target = intersects_entities(box, entities);
					if(target != nullptr) {
						velocity.x = 0;
					}
				}
				box.src.x = tempCoordinate;
				box.src.x += velocity.x;
			}

			if(velocity.y != 0.0) {
				entity.grounded = false;
				tempCoordinate = box.src.y;
				box.src.y += velocity.y;
				Vec3f volume = intersects_tree(staticTree, box, 0);
				if(volume.x > 0 && volume.y > 0 && volume.z > 0) {
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
				box.src.y = tempCoordinate + velocity.y;
			}

			if(velocity.z != 0.0) {
				tempCoordinate = box.src.z;
				box.src.z += velocity.z;
				Vec3f volume = intersects_tree(staticTree, box, 0);
				if(volume.x > 0 && volume.y > 0 && volume.z > 0) {
					velocity.z = 0.0;
				} else {
					Box *target = intersects_entities(box, entities);
					if(target != nullptr) {
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
	//std::cout << number_of_checks << std::endl;
	number_of_checks = 0;
}
