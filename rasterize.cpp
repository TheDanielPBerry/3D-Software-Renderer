#include <iostream>
#include <vector>
#include <cmath>

#include "Plane.h"



Vec2f interpolate_lines(
	Plane &plane,
	const u_char left[2],
	const u_char right[2],
	Vec2f &y_bounds,
	const Vec2f &dimensions,
	Uint32 *buffer,
	float *z_buffer,
	Vec2f starting_x
)
{
	const u_char pixel_size = plane.texture->pitch / plane.texture->w;
	#define START 0
	#define END 1


	Vec2f left_coeff_x, right_coeff_x;
	coeffs(ident(plane.buffer[left[START]], 1), ident(plane.buffer[left[END]], 1), left_coeff_x);
	coeffs(ident(plane.buffer[right[START]], 1), ident(plane.buffer[right[END]], 1), right_coeff_x);
	if((y_bounds.y - y_bounds.x) < 1) {
		//If either of the slopes are near horizontal, we don't need to draw it
		return Vec2f{-1, -1};
	}

	Vec2f left_coeff_perspective, right_coeff_perspective;
	coeffs(Vec2f { 1 / plane.buffer[left[START]].z, plane.buffer[left[START]].y }, 
		Vec2f { 1 / plane.buffer[left[END]].z, plane.buffer[left[END]].y }, left_coeff_perspective);
	coeffs(Vec2f { 1 / plane.buffer[right[START]].z, plane.buffer[right[START]].y }, 
		Vec2f { 1 / plane.buffer[right[END]].z, plane.buffer[right[END]].y }, right_coeff_perspective);


	//Interpolate each corners color
	Vec2f left_coeff_red, right_coeff_red;
	coeffs(Vec2f { plane.color[left[START]].x, plane.buffer[left[START]].y }, Vec2f { plane.color[left[END]].x, plane.buffer[left[END]].y }, left_coeff_red);
	coeffs(Vec2f { plane.color[right[START]].x, plane.buffer[right[START]].y }, Vec2f { plane.color[right[END]].x, plane.buffer[right[END]].y }, right_coeff_red);

	Vec2f left_coeff_green, right_coeff_green;
	coeffs(Vec2f { plane.color[left[START]].y, plane.buffer[left[START]].y }, Vec2f { plane.color[left[END]].y, plane.buffer[left[END]].y }, left_coeff_green);
	coeffs(Vec2f { plane.color[right[START]].y, plane.buffer[right[START]].y }, Vec2f { plane.color[right[END]].y, plane.buffer[right[END]].y }, right_coeff_green);

	Vec2f left_coeff_blue, right_coeff_blue;
	coeffs(Vec2f { plane.color[left[START]].z, plane.buffer[left[START]].y }, Vec2f { plane.color[left[END]].z, plane.buffer[left[END]].y }, left_coeff_blue);
	coeffs(Vec2f { plane.color[right[START]].z, plane.buffer[right[START]].y }, Vec2f { plane.color[right[END]].z, plane.buffer[right[END]].y }, right_coeff_blue);


	Vec2f left_coeff_texture_x, right_coeff_texture_x;
	Vec2f left_coeff_texture_y, right_coeff_texture_y;
	if(plane.texture != nullptr) {
		coeffs(Vec2f { (plane.texture_coords[left[START]].x * plane.texture->w) / plane.buffer[left[START]].z, 
			plane.buffer[left[START]].y }, 
		Vec2f { (plane.texture_coords[left[END]].x * plane.texture->w) / plane.buffer[left[END]].z, 
		 	plane.buffer[left[END]].y }, left_coeff_texture_x);

		coeffs(Vec2f { (plane.texture_coords[right[START]].x * plane.texture->w) / plane.buffer[right[START]].z, 
			plane.buffer[right[START]].y }, 
		 Vec2f { (plane.texture_coords[right[END]].x * plane.texture->w) / plane.buffer[right[END]].z, 
			 plane.buffer[right[END]].y }, right_coeff_texture_x);

		coeffs(Vec2f { (plane.texture_coords[left[START]].y * plane.texture->h) / plane.buffer[left[START]].z, 
			plane.buffer[left[START]].y}, 
		Vec2f { (plane.texture_coords[left[END]].y * plane.texture->h) / plane.buffer[left[END]].z, 
			 plane.buffer[left[END]].y }, left_coeff_texture_y);

		coeffs(Vec2f { (plane.texture_coords[right[START]].y * plane.texture->h) / plane.buffer[right[START]].z, 
			plane.buffer[right[START]].y }, 
		Vec2f { (plane.texture_coords[right[END]].y * plane.texture->h) / plane.buffer[right[END]].z, 
		 	plane.buffer[right[END]].y}, right_coeff_texture_y);
	}

	y_bounds.x = std::min(y_bounds.x, dimensions.y);
	y_bounds.y = std::max((float)0.0, y_bounds.y);

	//Set the left and right bounds to the second value on the function
	//If the slope is infinite it will use this for the entire range
	float x_left = left_coeff_x.y, x_right = right_coeff_x.y;

	uint initial_y = y_bounds.x;

	for(uint y=initial_y; y<y_bounds.y; y++) {
		uint yOffset = y * dimensions.x;

		if(y == initial_y && starting_x.x != -1) {
			x_left = starting_x.x;
		} else{
			x_left = line(left_coeff_x, y);
		}
		if(std::isnan(x_left)) {
			return Vec2f{-1, -1};
		}

		if(y == initial_y && starting_x.y != -1) {
			x_right = starting_x.y;
		} else {
			x_right = line(right_coeff_x, y);
		}
		if(std::isnan(x_right)) {
			return Vec2f{-1, -1};
		}


		Vec2f perspective_coeff;
		float left_perspective = line(left_coeff_perspective, y);
		float right_perspective = line(right_coeff_perspective, y);
		coeffs(Vec2f{right_perspective, x_right}, Vec2f{left_perspective, x_left}, perspective_coeff);

		//Color Interpolations
		float red_left = line(left_coeff_red, y);
		float red_right = line(right_coeff_red, y);
		Vec2f red_coeff;
		coeffs(Vec2f{red_right, x_right}, Vec2f{red_left, x_left}, red_coeff);

		float green_left = line(left_coeff_green, y);
		float green_right = line(right_coeff_green, y);
		Vec2f green_coeff;
		coeffs(Vec2f{green_right, x_right}, Vec2f{green_left, x_left}, green_coeff);

		float blue_left = line(left_coeff_blue, y);
		float blue_right = line(right_coeff_blue, y);
		Vec2f blue_coeff;
		coeffs(Vec2f{blue_right, x_right}, Vec2f{blue_left, x_left}, blue_coeff);

		Vec2f texture_x_coeff;
		Vec2f texture_y_coeff;
		if(plane.texture != nullptr) {
			float texture_x_left = line(left_coeff_texture_x, y);
			float texture_x_right = line(right_coeff_texture_x, y);
			coeffs(Vec2f{texture_x_right, x_right}, Vec2f{texture_x_left, x_left}, texture_x_coeff);

			float texture_y_left = line(left_coeff_texture_y, y);
			float texture_y_right = line(right_coeff_texture_y, y);
			coeffs(Vec2f{texture_y_right, x_right}, Vec2f{texture_y_left, x_left}, texture_y_coeff);
		}

		x_left = std::max((float)0.0, x_left);
		x_left = std::min(x_left, dimensions.x);
		x_right = std::max((float)0.0, x_right);
		x_right = std::min(x_right, dimensions.x);

		for(uint x=x_left; x<x_right; x++) {
			float perspective = line(perspective_coeff, x);
			float luminosity = 1.0;

			if(z_buffer[yOffset + x] < perspective) {
				float red = line(red_coeff, x);
				float green = line(green_coeff, x);
				float blue = line(blue_coeff, x);


				Uint32 pixel;
				if(plane.texture != nullptr) {
					int texture_coord_x = (line(texture_x_coeff, x) / (perspective));
					if(texture_coord_x < 0) {
						texture_coord_x = 1-texture_coord_x;
					}
					texture_coord_x %= plane.texture->w;

					int texture_coord_y = (line(texture_y_coeff, x) / (perspective));
					if(texture_coord_y < 0) {
						texture_coord_y = 1-texture_coord_y;
					}
					texture_coord_y %= plane.texture->h;

					int texture_offset = (plane.texture->pitch * texture_coord_y) + (texture_coord_x * pixel_size);
					if(((u_char *)plane.texture->pixels)[texture_offset + 3] == 0.0) {
						continue;
					}
					red *= ((u_char *)plane.texture->pixels)[texture_offset + 0];
					green *= ((u_char *)plane.texture->pixels)[texture_offset + 1];
					blue *= ((u_char *)plane.texture->pixels)[texture_offset + 2];

					pixel = ((uint)(std::min((red * luminosity),red)) << 24);
					pixel += ((uint)(std::min((green * luminosity), green)) << 16);
					pixel += ((uint)(std::min((blue * luminosity), blue)) << 8);
					pixel += 255;
				} else {
					pixel = ((uint)(std::min((red * luminosity),red) * 255) << 24);
					pixel += ((uint)(std::min((green * luminosity), green) * 255) << 16);
					pixel += ((uint)(std::min((blue * luminosity), blue) * 255) << 8);
					pixel += 255;
				}


				buffer[yOffset + x] = pixel;
				z_buffer[yOffset + x] = perspective;
			}
		}
	}
	return Vec2f{ x_left, x_right };
}


/**
* The plan for this function is to take points that are already projected and mapped for a buffer and rasterize them.
* I think the z_buffer will need to be filled with zeros the same as the main buffer
*/
void rasterize(Plane &plane, Uint32 *buffer, const Vec2f &dimensions, float *z_buffer)
{

	char top = 0, middle = 1, bottom = 2;
	//Test if all the 2d points are under off of the buffer in one direction or the other
	if(plane.buffer[top].x < 0.0 && plane.buffer[middle].x < 0.0 && plane.buffer[bottom].x < 0.0) {
		return;
	}
	if(plane.buffer[top].x > dimensions.x && plane.buffer[middle].x > dimensions.x && plane.buffer[bottom].x > dimensions.x) {
		return;
	}
	if(plane.buffer[top].y < 0.0 && plane.buffer[middle].y < 0.0 && plane.buffer[bottom].y < 0.0) {
		return;
	}
	if(plane.buffer[top].y > dimensions.y && plane.buffer[middle].y > dimensions.y && plane.buffer[bottom].y > dimensions.y) {
		return;
	}


	char temp;
	//Sort the points from top to bottom on the y axis
	if(plane.buffer[top].y > plane.buffer[middle].y) {
		temp = top;
		top = middle;
		middle = temp;
	}
	if(plane.buffer[middle].y > plane.buffer[bottom].y) {
		temp = bottom;
		bottom = middle;
		middle = temp;
	}
	if(plane.buffer[top].y > plane.buffer[middle].y) {
		temp = top;
		top = middle;
		middle = temp;
	}

	//Calculate coefficients for left and right lines
	//If the middle point is on one side of the line or the other will determine if the triangle is left or right oriented
	//Can calculate the side using the dot product of the 3 points in 2d
	bool leftOrientedTriangle = (((plane.buffer[bottom].x - plane.buffer[top].x) 
			* (plane.buffer[middle].y - plane.buffer[top].y))
			- ((plane.buffer[bottom].y - plane.buffer[top].y)
			* (plane.buffer[middle].x - plane.buffer[top].x))) > 0;

	
	Vec2f y_bounds = Vec2f {
		std::max(plane.buffer[top].y, (float)0.0),
		std::min(plane.buffer[middle].y, dimensions.y)
	};

	u_char left[2], right[2];
	if(leftOrientedTriangle) {
		left[0] = top;
		left[1] = middle;
		right[0] = top;
		right[1] = bottom;
	} else {
		left[0] = top;
		left[1] = bottom;
		right[0] = top;
		right[1] = middle;
	}
	Vec2f x_end = interpolate_lines(plane, left, right, y_bounds, dimensions, buffer, z_buffer, Vec2f{ -1, -1 });

	y_bounds = Vec2f {
		std::max(plane.buffer[middle].y, (float)0.0),
		std::min(plane.buffer[bottom].y, dimensions.y)
	};

	if(leftOrientedTriangle) {
		left[0] = middle;
		left[1] = bottom;
	} else {
		right[0] = middle;
		right[1] = bottom;
	}
	interpolate_lines(plane, left, right, y_bounds, dimensions, buffer, z_buffer, x_end);
}

void draw_scene(std::vector<Plane> &scene, Uint32 *buffer, const Vec2f &dimensions, const Vec3f &translate, const Vec3f &rotate, float *z_buffer)
{
	const Vec3f rotationTrig[2] = {
		Vec3f{cos(rotate.x), cos(rotate.y), 0},
		Vec3f{sin(rotate.x), sin(rotate.y), 0},
	};

	Plane splits[2];

	for(uint p=0; p<scene.size(); p++) {
		if(!transform(scene[p], translate, rotationTrig)) {
			continue;
		}

		int split_plane_count = clip_plane(scene[p], splits);
		for(int s=0; s<split_plane_count; s++) {
			project_and_scale(splits[s], dimensions);
			rasterize(splits[s], buffer, dimensions, z_buffer);
		}
	}
}
