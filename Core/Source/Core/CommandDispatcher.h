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

#include "GameEntity.h"

class CommandDispatcher {
private: 
	/*
		Store references to all registered entities
	*/
	std::vector<GameEntity*> entities;
public:
	CommandDispatcher();

	/*
		Deconstructor for the command dispatcher. Will clear all registered entities. 
	*/
	~CommandDispatcher();

	/*
		Register an entity with the command dispatcher. 
	*/
	void addEntity(GameEntity* entity);

	/*
		Deregister an entity with the command dispatcher. 
	*/
	void removeEntity(GameEntity* entity);

	/*
		Send a command from this entity to another. Commands are structs. 
	*/
	void dispatchCommand(Command command);
};