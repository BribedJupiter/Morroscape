// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <raylib.h>
#include <iostream>

#include "GameManager.h"

// TODO: Fix always initialize a localPlayerController

GameManager::GameManager(bool server, bool drawDebug)
	: server(server), drawDebug(drawDebug), dispatcher(CommandDispatcher()), world(World(dispatcher)),
	localPlayerController(PlayerController(dispatcher))
{
	// Setup entities
	this->setName("Game Manager");
	world.setName("World");
	localPlayerController.setName("Local Player Controller");

	// Setup dispatcher
	dispatcher.addEntity(this); // add game manager
	dispatcher.addEntity(&world); // add world
	dispatcher.addEntity(&localPlayerController); // add the player controller

	// Open window
	SetConfigFlags(FLAG_MSAA_4X_HINT); // enable anti-aliasing
	InitWindow(screenWidth, screenHeight, "Morroscape");

	// Setup world - must be done after initializing the window as it loads objects into the GPU
	world.populate();

	if (server) {
		// If server
	}
	else {
		// If client
		dispatcher.dispatchCommand({ "World", "Local Player Controller", "SWITCH CAMERA FIRST PERSON", nullptr });
	}

	// Final initialization
	DisableCursor();
	SetTargetFPS(TARGET_FPS);
}

void GameManager::dispatchCommand(Command command) {
	if (command.message == "CLEAR WORLD") {
		world.clearWorld();
	}
	if (command.message == "CLEAR SPAWNED") {
		world.clearSpawned();
	}
	if (command.message == "POPULATE") {
		world.populate();
	}
}

void GameManager::connectToNetwork() {

}

void GameManager::syncWithNetwork() {

}

void GameManager::update() {
	float deltaTime = GetFrameTime();
	localPlayerController.update(deltaTime);
	world.update(deltaTime); // update world and physics
}

void GameManager::render() {
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode3D(localPlayerController.getCameraController().getCamera());

	if (drawDebug) {
		DrawGrid(100, 1.0f);
		DrawLine3D({ 0, 0, 0 }, { 0, 100, 0 }, RED);
	}

	world.render(drawDebug); // handle 3D, render world and phsyics

	EndMode3D();

	if (drawDebug) {
		(localPlayerController.getCameraController().getCameraMode() == CAMERA_FIRST_PERSON)
			? DrawText("First Person", 10, 40, 20, DARKGREEN) : localPlayerController.getCameraController().getCameraMode() == CAMERA_THIRD_PERSON
			? DrawText("Third Person", 10, 40, 20, GREEN) : DrawText("Free Camera", 10, 40, 20, LIME);
		DrawText("Morroscape v0.01", 10, screenHeight - 30, 20, RED);
		DrawFPS(10, 10);
	}

	EndDrawing();
}

GameManager::~GameManager() {
	CloseWindow();
}

