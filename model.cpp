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
	std::vector<Box> boxes;
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

void load_bounding_boxes()
{

}

SDL_Surface *load_mtl(std::string filePath, Model &model, std::vector<SDL_Surface *> &texture_pool)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return NULL;
	}

	std::string line;
	while(std::getline(inputFile, line)) {
		int space = -1;
		std::string firstWord;
		tie(firstWord, space) = next_word(line, 0);
		if(firstWord == "map_Kd") {
			std::string imgPath = line.substr(space+1);
			uint pos = imgPath.find("assets/");
			std::string imgRelative = imgPath.substr(pos);
			std::pair<SDL_Surface *, uint> material = load_texture(imgRelative.c_str(), texture_pool);
			model.texture = material.second;
			return material.first;
		}
	}
	return NULL;
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
					v.x = coord;
				} else if(count == 1) {
					v.y = coord;
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
		} else if(firstWord == "mtllib") {
			int lastSeparator = filePath.find_last_of("/");
			std::string fileRoute = filePath.substr(0, lastSeparator+1);
			fileRoute += line.substr(space+1);
			load_mtl(fileRoute, model, texture_pool);
		} else if(firstWord == "b") {
			Box box;

			std::string vertexStr;
			tie(vertexStr, space) = next_word(line, space+1);
			uint slash = space;
			std::string index;

			tie(index, slash) = next_word(vertexStr, '/', 0);
			float d = std::stof(index);
			box.pos.x = d;

			tie(index, slash) = next_word(vertexStr, '/', slash+1);
			d = std::stof(index);
			box.pos.y = d;

			tie(index, slash) = next_word(vertexStr, '/', slash+1);
			d = std::stof(index);
			box.pos.z = d;

			tie(vertexStr, space) = next_word(line, space+1);
			tie(index, slash) = next_word(vertexStr, '/', 0);
			d = std::stof(index);
			box.dim.x = d;

			tie(index, slash) = next_word(vertexStr, '/', slash+1);
			d = std::stof(index);
			box.dim.y = d;

			tie(index, slash) = next_word(vertexStr, '/', slash+1);
			d = std::stof(index);
			box.dim.z = d;

			model.boxes.push_back(box);
		}
	}

	models.push_back(model);
	return models.size()-1;
}

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::vector<SDL_Surface *> &texture_pool,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale,
	bool cullable,
	Entity *entity,
	bool cameraStatic
)
{
	Vec3f trig[2] = {
		Vec3f{cos(rotation.x), cos(rotation.y), cos(rotation.z)},
		Vec3f{sin(rotation.x), sin(rotation.y), sin(rotation.z)},
	};

	for(Box box : model.boxes) {
		box.pos = box.pos * scale;
		box.dim = box.dim * scale;
		box.entity = entity;
		if(entity != nullptr) {
			std::cout << model.boxes.size() << std::endl;
			entity->boxes.push_back(box);
		} else {

		}
	}

	for(int i=0; i<model.planes.size(); i++) {
		Plane plane = Plane{
			.points = {
				model.vertices[model.planes[i][0][0]],
				model.vertices[model.planes[i][0][1]],
				model.vertices[model.planes[i][0][2]],
			},
			.normals = {
				model.normals[model.planes[i][2][0]],
				model.normals[model.planes[i][2][1]],
				model.normals[model.planes[i][2][2]],
			},
			.normal = model.normals[model.planes[i][2][0]],
			.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
			.texture_coords = {
				model.texture_coords[model.planes[i][1][0]],
				model.texture_coords[model.planes[i][1][1]],
				model.texture_coords[model.planes[i][1][2]],
			},
			.texture = texture_pool[model.texture],
			.cameraStatic = cameraStatic,
		};


		for(uint p=0; p<N_POINTS; p++) {
			float x, y, z, temp_y;
			plane.points[p] = plane.points[p] * scale;
			rotate(plane.points[p], trig);
			plane.points[p] = plane.points[p] + pos;


			// scale normals and make offset of point
			plane.normals[p] = (plane.normals[p]) * scale;
			rotate(plane.normals[p], trig);
			plane.normals[p] = plane.normals[p] + plane.points[p];
		}


		plane.normal = plane.normals[0];

		plane.cullable = cullable;

		plane.entity = entity;

		scene.push_back(plane);
	}

}

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::vector<SDL_Surface *> &texture_pool,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale,
	bool cullable,
	Entity *entity
)
{
	add_model_to_scene(model, scene, texture_pool, pos, rotation, scale, cullable, entity, false);
}
