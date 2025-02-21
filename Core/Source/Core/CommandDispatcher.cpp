// --------------------------------------------------
// 
// Morroscape
// Created 1/8/2024
// Jack Bauer
// 
// --------------------------------------------------

#include <raylib.h>

#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher() {}

void CommandDispatcher::addEntity(GameEntity* entity) {
	entities.push_back(entity);
}

void CommandDispatcher::removeEntity(GameEntity* entity) {
	std::vector<GameEntity*>::iterator entityIndex = std::find(entities.begin(), entities.end(), entity);
	if (!(entityIndex != entities.end())) {
		// go is not in vector
		std::cout << "[CommandDispatcher] could not find entity in entities" << std::endl;
		return;
	}
	entities.erase(entityIndex);
}

void CommandDispatcher::dispatchCommand(Command command) {
	for (GameEntity* entity : entities) {
		if (entity->name == command.target) {
			entity->receiveCommand(command);
			break;
		}
	}
}

CommandDispatcher::~CommandDispatcher() {
	entities.clear();
}