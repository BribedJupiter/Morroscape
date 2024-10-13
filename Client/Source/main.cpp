// --------------------------------------------------
// 
// Morroscape
// Created 12/31/2023
// Jack Bauer
// 
// --------------------------------------------------
#include <vector>
#include <string>
#include <iostream>

#include <raylib.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "World/World.h"
#include "Manager/GameManager.h"


// TODO: Implement debug world for physics
// TODO: Fix memory issue -> if populate world multiple times, and clear it, memory usage will not decrease as much
// TODO: Fix --> there are not checks when copying GameObject that what is copied as the physicsObject is actually the physicsObject
// TODO: Fix --> tracking size in both when spawned and physicsdetails is redundant
// TODO: Test CommandDispatcher::remove

// rock paper scissors combat

// Server flow
// Main --> init server --> init GameManager for server --> 

// Client flow
// Main --> init client --> init GameManager for client --> open window --> init world ...

// main - manage the highest level game state - decide if server or client, if render game or just command line
// get server / client to where they need to be

// Intermediary
// GameManager --> manage the non-physical aspects of the player and game state, dispatch commands,
// and communicated with network

// Local
// World --> manage the game world and its game objects and the physical presence of players
// PhysicsWorld --> manage the physics part of the world
// GameObject --> something that affects or has a presence in the game world
// PhysicsObject --> the physics related component of a game object
// PlayerController --> handle input, player actions, and update camera
// CameraController --> manage the camera. The player's window into the world

int main() {

	// Initialization
	// --------------------------------------------------
	bool isServer = true;
	GameManager gameManager = GameManager(isServer);
	// --------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) {
		// Update
		// --------------------------------------------------
		gameManager.update();

		// Render
		// --------------------------------------------------
		gameManager.render();
	}

	// De-initialization
	// --------------------------------------------------
	return 0;
}