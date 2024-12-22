#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Plane.h"

typedef struct Model {
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texture_coords;
	std::vector<std::vector<std::vector<int>>> planes;
	uint texture;
} Model;


/**
* @return identifier to texture
*/
std::pair<SDL_Surface *, uint> load_texture(const char *filePath, std::vector<SDL_Surface *> &texture_pool)
{
	SDL_Surface *texture = IMG_Load(filePath);
	SDL_LockSurface(texture);
	texture_pool.push_back(texture);
	return std::make_pair(texture, texture_pool.size()-1);
}


std::pair<std::string, int> next_word(std::string line, char delimiter, uint offset)
{
	uint space = line.find(delimiter, offset);
	return make_pair(line.substr(offset, space-offset), space);
}

std::pair<std::string, int> next_word(std::string line, uint offset)
{
	return next_word(line, ' ', offset);
}

int load_obj_model(
	std::string filePath, 
	std::vector<Plane> &scene, 
	std::vector<SDL_Surface *> &texture_pool, 
	std::vector<Model> &models
)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return -1;
	}

	Model model;

	std::string line;
	while(std::getline(inputFile, line)) {
		int space = -1;
		std::string firstWord;
		tie(firstWord, space) = next_word(line, 0);
		if(firstWord == "v") {
			Vec3f v;
			std::string coordStr;
			tie(coordStr, space) = next_word(line, space+1);
			uint count = 0;
			float scale = 0.1;
			while(true) {
				float coord = std::stof(coordStr);
				if(count == 0) {
					v.x = -coord;
				} else if(count == 1) {
					v.y = -coord;
				} else if(count == 2) {
					v.z = coord;
				}
				if(space < 0) {
					break;
				}
				tie(coordStr, space) = next_word(line, space+1);
				count++;
			}
			model.vertices.push_back(v);

		} else if(firstWord == "vn") {
			Vec3f vn;
			std::string coordStr;
			tie(coordStr, space) = next_word(line, space+1);
			uint count = 0;
			while(true) {
				float coord = std::stof(coordStr);
				if(count == 0) {
					vn.x = coord;
				} else if(count == 1) {
					vn.y = coord;
				} else if(count == 2) {
					vn.z = coord;
				}
				if(space < 0) {
					break;
				}
				tie(coordStr, space) = next_word(line, space+1);
				count++;
			}
			model.normals.push_back(vn);
		} else if(firstWord == "vt") {
			Vec2f vt;
			std::string coordStr;
			tie(coordStr, space) = next_word(line, space+1);
			uint count = 0;
			while(true) {
				float coord = std::stof(coordStr);
				if(count == 0) {
					vt.x = coord;
				} else if(count == 1) {
					vt.y = 1-coord;
				}
				if(space < 0) {
					break;
				}
				tie(coordStr, space) = next_word(line, space+1);
				count++;
			}
			model.texture_coords.push_back(vt);

		} else if(firstWord == "f") {
			std::vector<int> verts;
			std::vector<int> text;
			std::vector<int> norms;
			verts.reserve(3);
			text.reserve(3);
			norms.reserve(3);

			std::string vertexStr;
			tie(vertexStr, space) = next_word(line, space+1);
			while(vertexStr.length() > 0) {
				uint slash = space;
				std::string index;
				tie(index, slash) = next_word(vertexStr, '/', 0);
				int i = std::stoi(index);
				verts.push_back(i-1);

				tie(index, slash) = next_word(vertexStr, '/', slash+1);
				i = std::stoi(index);
				text.push_back(i-1);

				tie(index, slash) = next_word(vertexStr, '/', slash+1);
				i = std::stoi(index);
				norms.push_back(i-1);

				if(space == -1) {
					break;
				}
				tie(vertexStr, space) = next_word(line, space+1);
			}
			std::vector<std::vector<int>> face;
			face.reserve(3);
			face.push_back(verts);
			face.push_back(text);
			face.push_back(norms);

			model.planes.push_back(face);
		}
	}


	std::pair<SDL_Surface *, uint> material = load_texture("assets/img/chest_texture.png", texture_pool);
	model.texture = material.second;

	models.push_back(model);
	return models.size()-1;
}

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::vector<SDL_Surface *> &texture_pool,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale
)
{
	for(int i=0; i<model.planes.size(); i++) {
		Plane plane = Plane{
			.points = {
				model.vertices[model.planes[i][0][0]],
				model.vertices[model.planes[i][0][1]],
				model.vertices[model.planes[i][0][2]],
			},
			.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
			.texture_coords = {
				model.texture_coords[model.planes[i][1][0]],
				model.texture_coords[model.planes[i][1][1]],
				model.texture_coords[model.planes[i][1][2]],
			},
			.texture = texture_pool[model.texture],
			.orientation = 1,
		};
		Vec3f normal;

		Vec3f cosine = Vec3f{cos(rotation.x), cos(rotation.y), cos(rotation.z)};
		Vec3f sine = Vec3f{sin(rotation.x), sin(rotation.y), sin(rotation.z)};
		for(uint p=0; p<N_POINTS; p++) {
			normal = normal + model.normals[p];
			//Scale everything
			plane.points[p].x = plane.points[p].x * scale.x;
			plane.points[p].y = plane.points[p].y * scale.y;
			plane.points[p].z = plane.points[p].z * scale.z;


			//Rotate around the y-axis first
			float x, y, z;
			x = (plane.points[p].x * cosine.y) - (plane.points[p].z * sine.y);
			z = (plane.points[p].z * cosine.y) + (plane.points[p].x * sine.y);

			//Then the x-axis
			y = (plane.points[p].y * cosine.x) - (z * sine.x);
			z = (z * cosine.x) + (plane.points[p].y * sine.x);

			//Lastly the z-axis
			y = (y * cosine.z) - (x * sine.z);
			x = (x * cosine.z) + (y * sine.z);

			plane.points[p] = Vec3f{x, y, z} + pos;
		}

		normal.x = normal.x / 3;
		normal.y = normal.y / 3;
		normal.z = normal.z / 3;
		float x, y, z;
		x = (normal.x * cosine.y) - (normal.z * sine.y);
		z = (normal.z * cosine.y) + (normal.x * sine.y);

		//Then the x-axis
		y = (normal.y * cosine.x) - (z * sine.x);
		z = (z * cosine.x) + (normal.y * sine.x);

		//Lastly the z-axis
		y = (y * cosine.z) - (x * sine.z);
		x = (x * cosine.z) + (y * sine.z);

		plane.normal = Vec3f{x, y, z} + pos;



		scene.push_back(plane);
	}

}
