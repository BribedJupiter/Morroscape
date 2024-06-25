// --------------------------------------------------
// 
// Morroscape
// Created 1/8/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <vector>
#include <raylib.h>
#include <string>
#include <iostream>

// To hold a command
struct Command {
	std::string sender;
	std::string target;
	std::string message;
	void* address;
};

// The base class for all game objects
class GameEntity {
public:
	std::string name; // inits to "UNNAMED"
	GameEntity();
	void setName(std::string name);
	virtual void receiveCommand(Command command);
};
