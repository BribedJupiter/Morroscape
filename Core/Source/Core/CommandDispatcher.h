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
	std::vector<GameEntity*> entities;
public:
	CommandDispatcher();
	~CommandDispatcher();
	void addEntity(GameEntity* entity);
	void removeEntity(GameEntity* entity);
	void dispatchCommand(Command command);
};