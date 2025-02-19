#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <cstring>
#include <iostream>
#include <thread>

#include "server.h"
#include "game.h"
#include "Plane.h"
#include "Physics.h"
#include "Light.h"
#include "scene.h"



namespace asio = boost::asio;
using boost::asio::ip::udp;


struct ConnectionDetails {
	std::string username;
	std::string passcode;
	std::string host;
	unsigned short port = 9001;
	int status = 0;
};


struct ConnectionDetails parse_connection_string(std::string details)
{
	ConnectionDetails cd;

	//Split host/port from user/pass
	const unsigned short at = details.find('@');
	std::string host;
	if(at >= 0) {
		std::string creds = details.substr(0, at);
		const unsigned short colon = creds.find(':');
		if(colon >= 0) {
			cd.username = creds.substr(0, colon);
			cd.passcode = creds.substr(colon+1);
		} else {
			cd.username = creds;
		}
		host = details.substr(at+1);
	} else {
		host = details;
	}

	const unsigned short colon = host.find(':');
	if(colon >= 0) {
		cd.host = host.substr(0, colon);
		std::string portStr = host.substr(colon+1);
		int portNum =  std::stoi(portStr);
		if(portNum < 1000 && portNum > 65535) {
			cd.port = (unsigned short) portNum;
		}
	} else {
		cd.host = host;
	}

	return cd;
}


void update_state(WorldStateResponse *response, struct ConnectionDetails *connection_details)
{
	std::cout << "CLIENT\n\n";
	asio::io_context io_context;
	udp::endpoint endpoint(asio::ip::make_address(connection_details->host), connection_details->port);
	udp::socket socket(io_context);
	socket.connect(endpoint);

	while(true) {
		WorldStateRequest request = WorldStateRequest {
			.action = Join,
			.signals = 0,
			.rotation = Vec3f{-420, 2, 2},
			.rotationalVelocity = Vec3f{-69, 2, 2},
		};

		socket.send(asio::buffer(&request, sizeof(request)));
		socket.receive(asio::buffer(response, sizeof(WorldStateResponse)));
	}
}


int main(int argc, char *argv[])
{
	struct ConnectionDetails connection_details;
	if(argc > 1) {
		unsigned short position = 0;
		unsigned char character = argv[1][position];
		while(character != '\0' && position < 512) {
			character = argv[1][++position];
		}
		argv[1][position] = '\0';

		std::string connectionString(argv[1]);
		connection_details = parse_connection_string(connectionString);
		//std::cout << connection_details.username << " | " << connection_details.passcode << " | " << connection_details.host << " | " << connection_details.host << " | " << connection_details.port;
	} else {
		std::cout << "Connection details required as argument: <user>:<passcode>@<host>:<port>" << std::endl;
		return 0;
	}

	WorldStateResponse response;
	std::thread net(update_state, &response, &connection_details);


	std::vector<Plane> scene;
	std::unordered_map<std::string, SDL_Surface *> texture_pool;
	std::vector<Box> staticBoxes;
	scene.reserve(8192);
	texture_pool.reserve(64);
	staticBoxes.reserve(1024);
	build_scene(scene, texture_pool, response.entities, staticBoxes);
	texture_pool.clear();

	std::random_shuffle(staticBoxes.begin(), staticBoxes.end());
	Box *staticTree = nullptr;
	for(Box &box : staticBoxes) {
		staticTree = insert_box(staticTree, box, 0);
	}

	std::vector<Light> lights;
	some_lights(lights);
	light_scene(scene, lights);

	Entity *camera = &(response.entities[1]);

	int finish = init_game(scene, camera, staticTree, response.entities, lights, staticBoxes);

	response.entities.clear();
	scene.clear();
	staticBoxes.clear();

	return finish;
	

	return 0;
}

