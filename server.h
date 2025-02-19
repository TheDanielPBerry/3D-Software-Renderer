#ifndef SERVER_H
#define SERVER_H

#include <vector>


#include "Physics.h"
#include "game.h"


#define MAX_PLAYERS 8
#define MAX_NAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 32

typedef enum OnlinePlayerStatus {
	Alive,
	Dead,
	Disconnected,
} OnlinePlayerStatus;

typedef enum CommType {
	Join,
	Disconnect,
	WorldState,
} ResponseType;


typedef struct OnlinePlayer {
	char name[MAX_NAME_LENGTH];
	OnlinePlayerStatus status = Alive;
} OnlinePlayer;


typedef struct WorldStateRequest {
	CommType action = WorldState;
	short signals = 0;
	Vec3f rotation;
	Vec3f rotationalVelocity;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & action;
		ar & signals;
		ar & rotation;
		ar & rotationalVelocity;
	}
} WorldStateRequest;

typedef struct WorldStateResponse {
	CommType action = WorldState;
	std::vector<Entity> entities;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & action;
		ar & entities;
	}
} WorldStateResponse;


typedef struct JoinRequest {
	CommType action = Join;
	char name[MAX_NAME_LENGTH];
	char passcode[MAX_PASSWORD_LENGTH];
} JoinRequest;


typedef struct JoinResponse {
	CommType action = Join;
} JoinResponse;

#endif
