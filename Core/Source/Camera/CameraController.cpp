// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------

#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "CameraController.h"

CameraController::CameraController()
	: camera({ 0 }), cameraSpeed(BASE_CAMERA_SPEED), cameraMode(CAMERA_FREE)
{
	// Camera setup
	camera.position = Vector3{ 4.0f, 2.0f, 0.0f };
	camera.target = Vector3{ 2.0f, 2.0f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::setCameraMode(CameraMode mode) {
	if (mode == CAMERA_FIRST_PERSON) {
		cameraMode = CAMERA_FIRST_PERSON;
		camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // reset roll
		// Put camera where target is, move target forward 
		float offsetX = camera.target.x - camera.position.x;
		float offsetY = camera.target.y - camera.position.y;
		float offsetZ = camera.target.z - camera.position.z;
		camera.position.x = camera.target.x;
		camera.position.y = 2.0f;
		camera.position.z = camera.target.z;
		camera.target = Vector3{camera.target.x + offsetX, camera.target.y + offsetY, camera.target.z + offsetZ};
	}
	if (mode == CAMERA_THIRD_PERSON) {
		cameraMode = CAMERA_THIRD_PERSON;
		camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // reset roll
		// Put target where camera is, move camera back
		float offsetX = camera.position.x - camera.target.x;
		float offsetY = camera.position.y - camera.target.y;
		float offsetZ = camera.position.z - camera.target.z;
		camera.target = camera.position;
		camera.position = Vector3{camera.position.x + offsetX, camera.position.y + offsetY, camera.position.z + offsetZ};
	}
	if (mode == CAMERA_FREE) {
		cameraMode = CAMERA_FREE;
	}
}

void CameraController::update(float deltaTime, Vector3 playerPos, Vector3 changePos) {
	float cameraSpeed = BASE_CAMERA_SPEED * deltaTime;
	//std::cout << "Camera position: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;
	//std::cout << "Player pos: (" << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << ") Target: (" << camera.target.x << ", " << camera.target.y << ", " << camera.target.z << ")" << " Change pos: (" << changePos.x << ", " << changePos.y << ", " << changePos.z << ")" << std::endl;
	/*
		1st person - playerObject at camera position. Camera position matches object position. Camera target does not.
			- camera target needs to match movement changes in player position, without being jittery
				Inputs: change in player position
			- camera target also needs to be updated by mouse movement
					Inputs: change in mouse position
			- camera position needs to match player position
					Inputs: change in player position
			- player rotation needs to match mouse movement
		3rd person - playerObject at camera target. Camera target matches object position. Camera position does not.

		GameObject -> apply forces with keys to move player. Camera follows object. 
			- input handler needs to be able to send a command to apply a force
			- camera needs to update accordingly

		Handle input in PlayerController. Use player object ref to apply force to physics object's rigidbody.
		Camera controller will be given an updated position of the playerObject and apply changes acordingly.
	*/

	// Update Camera for first person
	if (cameraMode == CAMERA_FIRST_PERSON)
		// Move camera to the position of target, and reset target
		camera.position = playerPos;
		camera.target = Vector3Add(camera.target, changePos);
		UpdateCameraPro(&camera,
			{0, 0, 0},
			Vector3{ GetMouseDelta().x * BASE_CAMERA_SPEED, GetMouseDelta().y * BASE_CAMERA_SPEED, 0.0f },
			0.0f
		);
		
	// TODO: Fixing 3rd person might need a rewrite of the whole camera module
	if (cameraMode == CAMERA_THIRD_PERSON) {
		// Handle camera movement in 3rd person
		camera.target = playerPos;
		camera.position = Vector3Add(camera.position, changePos);
		UpdateCamera(&camera, CAMERA_THIRD_PERSON);
	}

	if (cameraMode == CAMERA_FREE)
		UpdateCamera(&camera, cameraMode);
}

Camera CameraController::getCamera() {
	return camera;
}

int CameraController::getCameraMode() {
	return cameraMode;
}

CameraController::~CameraController() {

}