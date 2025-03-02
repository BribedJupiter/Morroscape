// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <raylib.h>
#include <string>
#include <vector>

#include "World/World.h"
#include "Player/PlayerController.h"
#include "Core/CommandDispatcher.h"
#include "Core/GameEntity.h"
#include "Networking/Server.h"
#include "Networking/Client.h"

// Game constants
const int TARGET_FPS = 120;
const bool DEBUG_MODE = true;

// CLient constants
const int screenWidth = 1280;
const int screenHeight = 720;

/* 
	The game manager object. It controls the state of the game itself, and should delegate to subsystems for the actual game related behavior. 
*/
class GameManager : public GameEntity {
private:
	bool isServer; //  server or client
	bool drawDebug = DEBUG_MODE; 

	// Register a top level command dispatcher for communication across entities
	CommandDispatcher dispatcher;

	// World 
	World world;

	// A reference to the local player controller. May need refactoring to support multiplayer. 
	PlayerController localPlayerController;

	// If we are running in server mode, this will be the object to control the server state. 
	Server server;

	// If we are running in client mode, store the client state. This should be the default case. 
	Client client;
public:
	/*
		Send commands from GameManager to other entities. 
	*/
	void dispatchCommand(Command command);
	void connectToNetwork();
	void syncWithNetwork();
	void update();
	void render();
	GameManager(bool isServer = false, bool drawDebug = true); // defaults to client behavior
	~GameManager();
};