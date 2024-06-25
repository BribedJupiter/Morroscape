// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <raylib.h>

#include "Camera/CameraController.h"
#include "Core/GameObject.h"
#include "Core/CommandDispatcher.h"
#include "Core/GameEntity.h"

const float BASE_MOVEMENT_SPEED = 10.0f;

class PlayerController : public GameEntity {
private:
	CameraController cameraController;
	CommandDispatcher& dispatcher;
	float movementSpeed;
	bool server;
	GameObject* playerObject_ptr;
	bool isPlayerSpawned;
	Vector3 lastPlayerPos;
public:
	PlayerController(CommandDispatcher& dispatcher, bool server=false);
	CameraController& getCameraController();
	
	void getPlayerObject();
	
	void receiveCommand(Command command);
	void update(float deltaTime);
	
	~PlayerController();
};