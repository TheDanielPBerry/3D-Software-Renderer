#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include "server.h"

#include "Vec.h"
#include "Physics.h"
#include "Plane.h"
#include "scene.h"

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


namespace asio = boost::asio;
using boost::asio::ip::udp;

typedef struct Client {
	asio::ip::address address;
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


Client *new_player(std::vector<Entity> &entities, std::vector<Client> &clients, asio::ip::address address)
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

int main(int argc, char *argv[])
{
	//Initialize the game world
	std::vector<Plane> scene;
	std::unordered_map<std::string, SDL_Surface *> texture_pool;
	WorldStateResponse state = WorldStateResponse {
		.action = WorldState,
		.entities = std::vector<Entity>(),
	};
	std::vector<Box> staticBoxes;
	scene.reserve(20000);
	texture_pool.reserve(64);
	staticBoxes.reserve(1024);
	build_scene(scene, texture_pool, state.entities, staticBoxes);

	//Dispose of rendering stuff for now
	texture_pool.clear();
	scene.clear();


	std::vector<Client> clients;
	clients.reserve(MAX_PLAYERS);

	OnlinePlayer players[MAX_PLAYERS] = {
		OnlinePlayer{

		},
	};
	
	//Initialize server
	std::cout << "SERVER Initiating\n\n";
	asio::streambuf input;
	asio::io_context io_context;
	udp::socket socket(io_context, udp::endpoint(udp::v4(), 9001));
	while(true) {
		udp::endpoint endpoint;
		WorldStateRequest request;
		std::size_t length = socket.receive_from(asio::buffer(&request, sizeof(request)), endpoint);

		socket.send_to(asio::buffer(&state, sizeof(state)), endpoint);
	}



	return 0;
}
