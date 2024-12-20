#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Plane.h"

void load_obj_model(std::string filePath, std::vector<Plane> scene, Entity *entity)
{
	std::ifstream inputFile(filePath);
	if(!inputFile.is_open()) {
		std::cerr << "Cannot open: " << filePath << std::endl;
		return;
	}

	std::string line;
	if(true) {

	}
}
