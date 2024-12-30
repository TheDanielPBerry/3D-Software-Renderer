#include <SDL2/SDL_scancode.h>
#include <iostream>

#include "game.h"
#include "lines.h"
#include "Physics.h"


void draw_box(
	Box &box,
	const Vec3f &translate,
	Vec3f (&rotationTrig)[2],
	const Vec2f &dimensions,
	Uint32 *buffer,
	float *z_buffer
)
{
	Line subject = Line{
		.box = &box,
	};
	subject.points[0] = box.pos;
	subject.points[1] = Vec3f{ box.pos.x + box.dim.x, box.pos.y, box.pos.z };
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[1].x = box.pos.x;
	subject.points[1].y = box.pos.y + box.dim.y;
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[1].y = box.pos.y;
	subject.points[1].z = box.pos.z + box.dim.z;
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}



	subject.points[0] = box.pos + box.dim;
	subject.points[1] = Vec3f{ box.pos.x, box.pos.y + box.dim.y, box.pos.z + box.dim.z };
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[1].x = subject.points[0].x;
	subject.points[1].y = box.pos.y;
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[1].y = subject.points[0].y;
	subject.points[1].z = box.pos.z;
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[0] = Vec3f{box.pos.x, box.pos.y + box.dim.y, box.pos.z + box.dim.z};
	subject.points[1] = Vec3f{box.pos.x, box.pos.y + box.dim.y, box.pos.z };
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}

	subject.points[0] = Vec3f{box.pos.x + box.dim.x, box.pos.y, box.pos.z + box.dim.z};
	subject.points[1] = Vec3f{box.pos.x + box.dim.x, box.pos.y, box.pos.z };
	if(transform_line(subject, translate, rotationTrig)) {
		project_line(subject, dimensions);
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].x++;
		subject.buffer[1].x++;
		draw_line(subject, dimensions, buffer, z_buffer);
		subject.buffer[0].y++;
		subject.buffer[1].y++;
		draw_line(subject, dimensions, buffer, z_buffer);
	}
}

void draw_bounding_boxes(
	std::vector<Box> &boxes,
	std::vector<Entity> &entities,
	const Vec3f &translate,
	const Vec3f &rotate,
	const Vec2f &dimensions,
	Uint32 *screen_buffer,
	float *z_buffer
)
{
	Vec3f rotationTrig[2] = {
		Vec3f{cos(rotate.x), cos(rotate.y), 1},
		Vec3f{sin(rotate.x), sin(rotate.y), 0},
	};

	for(Box &box : boxes) {
		draw_box(box, translate, rotationTrig, dimensions, screen_buffer, z_buffer);
	}
	for(Entity &entity : entities) {
		for(Box &box : entity.boxes) {
			draw_box(box, translate, rotationTrig, dimensions, screen_buffer, z_buffer);
		}
	}
}

Box *box_of_interest;
bool pos_mode = true;
int box_axis = 0;
float box_edit_granularity = 0.5;

void set_box_of_interest(Box *box)
{
	box_of_interest = box;
}

void check_boxedit_keys(SDL_Event &event, Signals &signals)
{
	if(event.key.keysym.scancode == 87) {
		if(box_of_interest == nullptr) {
			return;
		}
		if(pos_mode) {
			box_of_interest->pos[box_axis] += box_edit_granularity;
		} else {
			box_of_interest->dim[box_axis] += box_edit_granularity;
		}
	} else if(event.key.keysym.scancode == 86) {
		if(box_of_interest == nullptr) {
			return;
		}
		if(pos_mode) {
			box_of_interest->pos[box_axis] -= box_edit_granularity;
		} else {
			box_of_interest->dim[box_axis] -= box_edit_granularity;
		}
	} else if(event.key.keysym.scancode == 40) {	//Enter Key
		if(box_of_interest == nullptr) {
			return;
		}
		std::cout << "\nb " << box_of_interest->pos.x << "/" << box_of_interest->pos.y << "/" << box_of_interest->pos.z;
		std::cout << " " << box_of_interest->dim.x << "/" << box_of_interest->dim.y << "/" << box_of_interest->dim.z << std::endl;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_P) {
		pos_mode = !pos_mode;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_X) {
		box_axis = 0;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_Y) {
		box_axis = 1;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_Z) {
		box_axis = 2;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_N) {
		box_edit_granularity /= 2;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_M) {
		box_edit_granularity *= 2;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_B) {
		signals.addBox = true;
	}
}

void box_signals(Signals &signals, std::vector<Box> &staticBoxes, Entity *camera, Box *staticTree)
{
	if(box_of_interest == nullptr) {
		box_of_interest = &staticBoxes[staticBoxes.size()-1];
	}
	if(signals.addBox) {
		signals.addBox = false;

		staticBoxes.push_back(Box{
			.pos = camera->pos,
			.dim = Vec3f{ 1, 1, 1 }
		});
		box_of_interest = &staticBoxes[staticBoxes.size()-1];
		//insert_box(staticTree, *box_of_interest);
	}

}
