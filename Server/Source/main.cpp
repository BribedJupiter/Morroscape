// --------------------------------------------------
// 
// Morroscape
// Created 7/20/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <vector>
#include <string>
#include <iostream>

#include <raylib.h>

#include "Manager/GameManager.h"

int main() {

	// Initialization
	// --------------------------------------------------
	bool server = true;
	GameManager gameManager = GameManager(server);

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