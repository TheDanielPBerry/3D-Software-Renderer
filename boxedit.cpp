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
	subject.points[0] = box.src;
	subject.points[1] = Vec3f{ box.dest.x, box.src.x, box.src.z };
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

	subject.points[1].x = box.src.x;
	subject.points[1].y = box.dest.y;
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

	subject.points[1].y = box.src.y;
	subject.points[1].z = box.dest.z;
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



	subject.points[0] = box.dest;
	subject.points[1] = Vec3f{ box.src.x, box.dest.y, box.dest.z };
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

	subject.points[1].x = box.dest.x;
	subject.points[1].y = box.src.y;
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

	subject.points[1].y = box.dest.y;
	subject.points[1].z = box.src.z;
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

	subject.points[0] = Vec3f{box.src.x, box.dest.y, box.dest.z};
	subject.points[1] = Vec3f{box.src.x, box.dest.y, box.src.z };
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

	subject.points[0] = Vec3f{ box.dest.x, box.src.y, box.dest.z};
	subject.points[1] = Vec3f{ box.dest.x, box.src.y, box.src.z };
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
uint box_index = 0;
bool pos_mode = true;
int box_axis = 0;
float box_edit_granularity = 0.5;

void set_box_of_interest(Box *box)
{
	box_of_interest = box;
}
Box *get_box_of_interest()
{
	return box_of_interest;
}

void check_boxedit_keys(SDL_Event &event, Signals &signals)
{
	if(event.key.keysym.scancode == 87) {
		if(box_of_interest == nullptr) {
			return;
		}
		if(pos_mode) {
			box_of_interest->src[box_axis] += box_edit_granularity;
		} else {
			box_of_interest->dest[box_axis] += box_edit_granularity;
		}
	} else if(event.key.keysym.scancode == 86) {
		if(box_of_interest == nullptr) {
			return;
		}
		if(pos_mode) {
			box_of_interest->src[box_axis] -= box_edit_granularity;
		} else {
			box_of_interest->dest[box_axis] -= box_edit_granularity;
		}
	} else if(event.key.keysym.scancode == 40) {	//Enter Key
		if(box_of_interest == nullptr) {
			return;
		}
		std::cout << "\nb " << box_of_interest->src.x << "/" << box_of_interest->src.y << "/" << box_of_interest->src.z;
		std::cout << " " << box_of_interest->dest.x << "/" << box_of_interest->dest.y << "/" << box_of_interest->dest.z << std::endl;
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
	} else if(event.key.keysym.scancode == SDL_SCANCODE_T) {
		signals.nextBox = true;
	} else if(event.key.keysym.scancode == SDL_SCANCODE_R) {
		signals.prevBox = true;
	}
}

void box_signals(Signals &signals, std::vector<Box> &staticBoxes, Entity *camera, Box *staticTree)
{
	if(box_of_interest == nullptr) {
		box_index = staticBoxes.size()-1;
		box_of_interest = &staticBoxes[box_index];
	}
	if(signals.addBox) {
		signals.addBox = false;

		staticBoxes.push_back(Box{
			.src = camera->pos,
			.dest = Vec3f{ 1, 1, 1 }
		});
		box_of_interest = &staticBoxes[staticBoxes.size()-1];
		//insert_box(staticTree, *box_of_interest);
	}
	if(signals.prevBox) {
		signals.prevBox = false;
		if(box_index > 0) {
			box_index--;
		}
		box_of_interest = &staticBoxes[box_index];
	}
	if(signals.nextBox) {
		signals.nextBox = false;
		if(box_index < staticBoxes.size()-1) {
			box_index++;
		}
		box_of_interest = &staticBoxes[box_index];
	}

}