#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>

#include "Plane.h"
#include "scene.h"

std::pair<std::string, int> next_word(std::string line, char delimiter, uint offset)
{
	uint space = line.find(delimiter, offset);
	return make_pair(line.substr(offset, space-offset), space);
}

std::pair<std::string, int> next_word(std::string line, uint offset)
{
	return next_word(line, ' ', offset);
}

void load_obj_model(std::string filePath, std::vector<Plane> &scene, Entity *entity, std::vector<SDL_Surface *> &texture_pool)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return;
	}

	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texture_coords;
	std::vector<std::vector<std::vector<int>>> planes;

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
					v.x = -coord * scale;
				} else if(count == 1) {
					v.y = -coord * scale;
				} else if(count == 2) {
					v.z = coord * scale;
				}
				if(space < 0) {
					break;
				}
				tie(coordStr, space) = next_word(line, space+1);
				count++;
			}
			vertices.push_back(v);

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
			normals.push_back(vn);
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
			texture_coords.push_back(vt);

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
			std::cout << verts.size() << " " << norms.size() << " " << " " << text.size() << std::endl;
			std::cout << face.size() << std::endl;

			planes.push_back(face);
		}
	}


	std::pair<SDL_Surface *, uint> shotgun = load_texture("assets/img/gun_metal.png", texture_pool);
	for(int i=0; i<planes.size(); i++) {
		scene.push_back(Plane{
			.points = {
				vertices[planes[i][0][0]],
				vertices[planes[i][0][1]],
				vertices[planes[i][0][2]],
			},
			.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
			.texture_coords = {
				texture_coords[planes[i][1][0]],
				texture_coords[planes[i][1][1]],
				texture_coords[planes[i][1][2]],
			},
			.texture = shotgun.first,
			.orientation = 1,
		});
	}
/*
	std::pair<SDL_Surface *, uint> brick = load_texture("assets/bricks.png", texture_pool);
	float x = 0.0, z= 0.0, y  = 0.0;
	scene.push_back(plane{
		.points = {
			{(float)(x + 0.5 - 0), (float)(y + 0.5), (float)(x + 0.5) + 2},
			{(float)(x + 0.5 - 0), (float)(y - 0.5), (float)(x + 0.5) + 2},
			{(float)(x - 0.5 - 0), (float)(y - 0.5), (float)(x - 0.5) + 2},
		},
		.color = {{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0},{1.0, 1.0, 1.0, 1.0}},
		.texture_coords = {{ 1, 1 }, { 1, 0 }, { 0 , 0 } },
		.texture = brick.first,
		.orientation = 1,
	});
	*/
}
