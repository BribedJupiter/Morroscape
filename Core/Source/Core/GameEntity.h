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

/*
	To store a command sent from one entity to another.
	Format: {std::string sender, std::string target, std::string message, void* address}
*/
struct Command {
	std::string sender; // The entity that is sending the message
	std::string target; // The entity that should receive the message. Referenced by name. 
	std::string message; // The message to send. Should follow a consistent convention. 
	void* address; // A pointer that may be necessary for certain commands. 
};

/* 
	The base class for all game objects. Entities should primarily store data and be acted upon by "systems", i.e. stateless functions. 
*/
class GameEntity {
public:
	/*
		The name registered with this entity. Used to reference it when sending commands. Will initialize to "UNNAMED".
	*/
	std::string name; 

	/*
		Construct an entity. By default, will initialize the entity's name to "UNNAMED"
	*/
	GameEntity();

	/*
		Change an entity's registered name. Changing this is bad practice. A name should only be set once. 
	*/
	void setName(std::string name);

	/*
		Handle commands when they're received. This needs to be reimplemented by each entity class. The default is to print to std::cout. 
	*/
	virtual void receiveCommand(Command command);
};
