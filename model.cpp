#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>
#include <unordered_map>

#include "Plane.h"
#include "Vec.h"

typedef struct Face {
	SDL_Surface *texture;
	std::vector<std::vector<int>> points;
} Face;

typedef struct Model {
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texture_coords;
	struct std::vector<Face> faces;
	std::vector<Box> boxes;
} Model;


/**
* @return identifier to texture
*/
SDL_Surface *load_texture(const char *filePath)
{
	SDL_Surface *texture = IMG_Load(filePath);
	SDL_LockSurface(texture);
	return texture;
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


void *load_mtl(std::string filePath, Model &model, std::unordered_map<std::string, SDL_Surface *> &texture_pool)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return NULL;
	}
	std::string materialName;
	std::string line;
	while(std::getline(inputFile, line)) {
		int space = -1;
		std::string firstWord;
		tie(firstWord, space) = next_word(line, 0);
		if(firstWord == "newmtl") {
			materialName = line.substr(space+1);
		} else if(firstWord == "map_Kd") {
			std::string imgPath = line.substr(space+1);
			uint pos = imgPath.find("assets/");
			std::string imgRelative = imgPath.substr(pos);
			SDL_Surface *material = load_texture(imgRelative.c_str());
			texture_pool[materialName] = material;
		}
	}
	return NULL;
}

int load_obj_model(
	std::string filePath, 
	std::vector<Plane> &scene, 
	std::unordered_map<std::string, SDL_Surface *> &texture_pool, 
	std::vector<Model> &models
)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return -1;
	}

	Model model;
	SDL_Surface *currentMaterial = nullptr;

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

			model.faces.push_back(Face {
				.texture = currentMaterial,
				.points = face,
			});
		} else if(firstWord == "mtllib") {
			int lastSeparator = filePath.find_last_of("/");
			std::string fileRoute = filePath.substr(0, lastSeparator+1);
			fileRoute += line.substr(space+1);
			load_mtl(fileRoute, model, texture_pool);
		} else if(firstWord == "usemtl") {
			std::string materialName;
			tie(materialName, space) = next_word(line, space+1);
			if(texture_pool.find(materialName) != texture_pool.end()) {
				currentMaterial = texture_pool[materialName];
			}
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

	inputFile.close();
	return models.size()-1;
}

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::unordered_map<std::string, SDL_Surface *> &texture_pool,
	std::vector<Box> &staticBoxes,
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
		rotate(box.pos, trig);

		box.dim = box.dim * scale;
		rotate(box.dim, trig);
		box.dim = box.dim;

		box.entity = entity;

		//Either associate all the boxes with an entity or add them into the oct tree world space
		if(entity != nullptr) {
			entity->boxes.push_back(box);
		} else {
			box.pos = box.pos + pos;
			staticBoxes.push_back(box);
		}
	}

	for(int i=0; i<model.faces.size(); i++) {
		Plane plane = Plane{
			.points = {
				model.vertices[model.faces[i].points[0][0]],
				model.vertices[model.faces[i].points[0][1]],
				model.vertices[model.faces[i].points[0][2]],
			},
			.normals = {
				model.normals[model.faces[i].points[2][0]],
				model.normals[model.faces[i].points[2][1]],
				model.normals[model.faces[i].points[2][2]],
			},
			.normal = model.normals[model.faces[i].points[2][0]],
			.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
			.texture_coords = {
				model.texture_coords[model.faces[i].points[1][0]],
				model.texture_coords[model.faces[i].points[1][1]],
				model.texture_coords[model.faces[i].points[1][2]],
			},
			.texture = model.faces[i].texture,
			.cameraStatic = cameraStatic,
		};



		for(uint p=0; p<N_POINTS; p++) {
			float x, y, z, temp_y;
			plane.points[p] = plane.points[p] * scale;
			rotate(plane.points[p], trig);
			plane.points[p] = plane.points[p] + pos;

		}

		plane.normal = normalize(cross_product(plane.points[0] - plane.points[1], plane.points[0]-plane.points[1]));

		plane.cullable = cullable;

		plane.entity = entity;

		scene.push_back(plane);
	}

}

void add_model_to_scene(
	Model &model,
	std::vector<Plane> &scene,
	std::unordered_map<std::string, SDL_Surface *> &texture_pool,
	std::vector<Box> &staticBoxes,
	Vec3f pos,
	Vec3f rotation,
	Vec3f scale,
	bool cullable,
	Entity *entity
)
{
	return add_model_to_scene(model, scene, texture_pool, staticBoxes, pos, rotation, scale, cullable, entity, false);
}
