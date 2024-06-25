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

// Game constants
const int TARGET_FPS = 120;
const bool DEBUG_MODE = true;

// CLient constants
const int screenWidth = 1280;
const int screenHeight = 720;

// Game manager controls the state of the game itself

class GameManager : public GameEntity {
private:
	bool server; //  server or client
	bool drawDebug = DEBUG_MODE; 
	CommandDispatcher dispatcher;
	World world;
	PlayerController localPlayerController;
public:
	GameManager(bool server=false, bool drawDebug=true); // defaults to client behavior
	void dispatchCommand(Command command);
	void connectToNetwork();
	void syncWithNetwork();
	void update();
	void render();
	~GameManager();
};