// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------

#include <raylib.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <raymath.h>

#include "PlayerController.h"

PlayerController::PlayerController(CommandDispatcher& dispatcher)
	: cameraController(CameraController()), dispatcher(dispatcher),
	movementSpeed(BASE_MOVEMENT_SPEED), playerObject_ptr(nullptr), isPlayerSpawned(false), lastPlayerPos({0, 0, 0})
{}

CameraController& PlayerController::getCameraController() {
	return cameraController;
}

void PlayerController::getPlayerObject() {
	dispatcher.dispatchCommand({ this->name, "World", "REQUEST PLAYER", nullptr });
}

void PlayerController::receiveCommand(Command command) {
	if (command.message == "SEND PLAYER SPAWNED") {
		//std::cout << "[Entity " << name << "] Player game object spawned located at " << command.address << std::endl;
		playerObject_ptr = (GameObject*) command.address; // cast to correct type
		isPlayerSpawned = true;
	}
	if (command.message == "SEND PLAYER DESPAWNED") {
		//std::cout << "[Entity " << name << "] Player game object despawned located at " << command.address << std::endl;
		playerObject_ptr = (GameObject*) command.address; // cast to correct type
		isPlayerSpawned = false;
	}
	if (command.message == "SWITCH CAMERA FIRST PERSON") {
		cameraController.setCameraMode(CAMERA_FIRST_PERSON);
	}
	//std::cout  << "[Entity " << name << "] Player object ptr " << playerObject_ptr << std::endl;
	else {
		//std::cout << "[Entity " << name << "] received command " << command.message << " from " << command.sender << " at address " << command.address << std::endl;
	}
}

void PlayerController::update(float deltaTime) {
	// Inputs
	// F - client - change perspective
	// WASD - move camera
	// Mouse - rotate camera

	// If 1st person -> player game object spawned, but not visible, the location of the camera follows the object's position
	// If 3rd person -> player game object spawned, camera's target is the object's position and cam pos follows too
	
	// Movement
	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		movementSpeed *= 2;
	}

	getPlayerObject(); // update what we know of the player
	if (isPlayerSpawned == true && cameraController.getCameraMode() != CAMERA_FREE) {
		playerObject_ptr->getPhysicsComponent().body->activate(true);
		Vector3 fwd = Vector3Normalize(Vector3Subtract(cameraController.getCamera().target, cameraController.getCamera().position));
		Vector3 right = Vector3CrossProduct(fwd, Vector3Normalize(cameraController.getCamera().up));

		if (IsKeyDown(KEY_W)) {
			playerObject_ptr->getPhysicsComponent().body->applyCentralImpulse({ fwd.x, 0.0, fwd.z });
		}
		if (IsKeyDown(KEY_S)) {
			playerObject_ptr->getPhysicsComponent().body->applyCentralImpulse({ -fwd.x, 0.0, -fwd.z });
		}
		if (IsKeyDown(KEY_A)) {
			playerObject_ptr->getPhysicsComponent().body->applyCentralImpulse({ -right.x, 0.0, -right.z });
		}
		if (IsKeyDown(KEY_D)) {
			playerObject_ptr->getPhysicsComponent().body->applyCentralImpulse({ right.x, 0.0, right.z });
		}
		if (IsKeyDown(KEY_SPACE)) {
			playerObject_ptr->getPhysicsComponent().body->applyCentralImpulse({ 0.0, 1.0, 0.0 });
		}
	}

	if (IsKeyPressed(KEY_PERIOD)) {
		// Tell the world to spawn the player object and generate its physics body
		dispatcher.dispatchCommand({ this->name, "World", "SPAWN PLAYER", nullptr });
	}

	if (IsKeyPressed(KEY_COMMA)) {
		dispatcher.dispatchCommand({ this->name, "World", "DESPAWN PLAYER", nullptr });
	}

	// Camera
	if (IsKeyPressed(KEY_F)) {
		cameraController.setCameraMode(
			cameraController.getCameraMode() == CAMERA_FIRST_PERSON 
			? CAMERA_THIRD_PERSON : CAMERA_FIRST_PERSON);
	}
	
	if (IsKeyPressed(KEY_P)) {
		cameraController.setCameraMode(CAMERA_FREE);
	}

	// Update camera
	getPlayerObject();
	if (isPlayerSpawned == true) {
		// Get the player's position this frame to compute the change in position
		btVector3 btPos = playerObject_ptr->getPhysicsComponent().body->getWorldTransform().getOrigin();
		Vector3 pos = { btPos.getX(), btPos.getY(), btPos.getZ() };
		Vector3 changePos = {pos.x - lastPlayerPos.x, pos.y - lastPlayerPos.y, pos.z - lastPlayerPos.z};

		// Set the last player position for use in the next iteration of update
		btVector3 oldBtPos = playerObject_ptr->getPhysicsComponent().body->getWorldTransform().getOrigin();
		lastPlayerPos = { oldBtPos.getX(), oldBtPos.getY(), oldBtPos.getZ() };

		// Update camera
		cameraController.update(deltaTime, pos, changePos);
	}
	else {
		cameraController.update(deltaTime, { 0, 4, 4 }, {0, 0, 0});
	}

}

PlayerController::~PlayerController() {
	// any cleanup
}