// --------------------------------------------------
// 
// Morroscape
// Created 1/8/2024
// Jack Bauer
// 
// --------------------------------------------------
#include "GameEntity.h"

GameEntity::GameEntity()
	: name("UNNAMED")
{}

void GameEntity::setName(std::string name) {
	this->name = name;
}

void GameEntity::receiveCommand(Command command) {
	std::cout << "[Entity " << name << "] received command " << command.message << " from " << command.sender << " at address " << command.address << std::endl;
}