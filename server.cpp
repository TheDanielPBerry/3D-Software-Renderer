#include <iostream>
#include <sstream>
#include <vector>

#include "garnet/src/Garnet.h"
#include "server.h"

#include "Vec.h"
#include "Physics.h"
#include "Plane.h"
#include "scene.h"

#include "boost/include/boost/archive/text_oarchive.hpp"
#include "boost/include/boost/archive/text_iarchive.hpp"


typedef struct Client {
	Garnet::Address address;
	struct Entity *entity;
} Client;


void tick_entities(std::vector<Client> clients)
{
}


void initialize_game()
{
}

Vec3f get_spawn(std::vector<Client> &clients)
{
	uint location = rand() % 4;
	const Vec3f spawn_points[4] = {
		Vec3f{0, 6, 0},
		Vec3f{-10, 6, 0},
		Vec3f{0, 6, -30},
		Vec3f{-20, 6, -30},
	};
	return spawn_points[location];
}


Client *new_player(std::vector<Entity> &entities, std::vector<Client> &clients, Garnet::Address address)
{
	entities.push_back(Entity {
		.pos = get_spawn(clients),
		.vel = Vec3f{},
		.rotation = Vec3f{},
		.rotational_velocity = Vec3f{},
		.type = Player
	});
	clients.push_back(Client{
		.address = address,
		.entity = &entities.back(),
	});
	return &clients.back();
}

constexpr uint float_offset(uint i)
{
	return sizeof(float) * i;
}

int main()
{
	//Initialize the game world
	std::vector<Plane> scene;
	std::unordered_map<std::string, SDL_Surface *> texture_pool;
	std::vector<Entity> entities;
	std::vector<Box> staticBoxes;
	scene.reserve(20000);
	texture_pool.reserve(64);
	staticBoxes.reserve(1024);
	build_scene(scene, texture_pool, entities, staticBoxes);
	texture_pool.clear();
	scene.clear();
	for(Entity &entity : entities) {
	}
	Entity *host = &entities[0];



	std::cout << "SERVER Initiating\n\n";
	Garnet::Init(true);
	Garnet::Socket serverSocket(Garnet::Protocol::UDP);
	serverSocket.bind(Garnet::Address{
		.host = "127.0.0.1",
		.port = 42069
	});


	std::vector<Client> clients;
	clients.reserve(MAX_PLAYERS);

	char buffer[1024];
	char *output = &buffer[0];

	Garnet::Address clientAddr;
	while (true) {
		Garnet::Address recvAddr;
		bool received = serverSocket.receiveFrom(buffer, sizeof(buffer), &recvAddr);
		if(received)
		{
			Client *client = new_player(entities, clients, recvAddr);
			clientAddr = recvAddr;
			if (strcmp(buffer, "!quit") == 0) {
				std::cout << "Client left the chat.\n";
				break;
			}
			else std::cout << "Client (" << clientAddr.host << ":" << clientAddr.port << "): " << buffer << "\n";
		}
		else continue;

		//Send Data Back
		std::cout << "Server: ";

		std::ostringstream archive_stream;
		boost::archive::text_oarchive oa(archive_stream);
		if(!serverSocket.sendTo(archive, sizeof(archive), clientAddr)) std::cout << "MESSAGE NOT SENT\n";
		
		std::cout << "uuid/action/vel/pos/rotationalVelocity/rotation/shot";

		output[0] = 'u';
		output[1] = entities.size();
		uint entity_offset = 0;
		for(Entity &entity : entities) {
			entity_offset += sizeof(entity);
		}
	}

	serverSocket.close();
	Garnet::Terminate();
	return 0;
}


