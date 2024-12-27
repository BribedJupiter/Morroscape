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

//**** Basic Helper functions ****//

// From rcamera.h
Vector3 GetCameraUp(Camera* camera) {
	return Vector3Normalize(camera->up);
}

// From rcamera.h
Vector3 GetCameraForward(Camera* camera) {
	return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}

// From rcamera.h
Vector3 GetCameraRight(Camera* camera) {
	Vector3 forward = GetCameraForward(camera);
	Vector3 up = GetCameraUp(camera);

	return Vector3Normalize(Vector3CrossProduct(forward, up));
}

//**** Advanced Helper Functions ****// 

// Yaw is side-to-side movement. Angle in radians. From rcamera.h. 
void CameraYaw(Camera *camera, float angle, bool rotateAroundTarget) {
	// Get rotation axis
	Vector3 up = GetCameraUp(camera);

	// View vector (vector camera is pointing)
	Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

	// Rotate view vector around up axis
	targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

	if (rotateAroundTarget) {
		camera->position = Vector3Subtract(camera->target, targetPosition);
	}
	else {
		camera->target = Vector3Add(camera->position, targetPosition);
	}
}

// Pitch is up/down movement. Angle in radians. lockView clamps rotation to avoid overrotation. From rcamera.h
void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget) {
	// up vector
	Vector3 up = GetCameraUp(camera);

	// View vector
	Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

	if (lockView) {
		// clamp look up 
		float maxAngleUp = Vector3Angle(up, targetPosition);	
		maxAngleUp -= 0.001f; // avoid numerical errors
		if (angle > maxAngleUp) angle = maxAngleUp;

		// clamp look down
		float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
		maxAngleDown *= -1.0f; // down angle is negative
		maxAngleDown += 0.001f; // avoid numerical errors
		if (angle < maxAngleDown) angle = maxAngleDown;
	}

	// Rotation axis
	Vector3 right = GetCameraRight(camera);

	// Rotate view vector around right axis
	targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

	if (rotateAroundTarget) {
		camera->position = Vector3Subtract(camera->target, targetPosition);
	}
	else {
		// rotate around camera pos
		camera->target = Vector3Add(camera->position, targetPosition);
	}
}

// Move the camera in the world. 
// Distance format is {forward, up, right} NOT {x, y, z}.
// Modified from rcamera.h
void CameraMove(Camera* camera, Vector3 distance) {
	// Get axis vectors
	Vector3 right = GetCameraRight(camera);
	Vector3 up = GetCameraUp(camera);
	Vector3 forward = GetCameraForward(camera);

	// project forward and right into world
	forward.y = 0;
	forward = Vector3Normalize(forward);
	right.y = 0;
	right = Vector3Normalize(right);

	// Scale vectors by distance
	forward = Vector3Scale(forward, distance.x);
	up = Vector3Scale(up, distance.y);
	right = Vector3Scale(right, distance.z);

	// Move camera's position sequentially
	// forward, up, right
	camera->position = Vector3Add(camera->position, forward);
	camera->position = Vector3Add(camera->position, up);
	camera->position = Vector3Add(camera->position, right);

	// Move target
	camera->target = Vector3Add(camera->target, forward);
	camera->target = Vector3Add(camera->target, up);
	camera->target = Vector3Add(camera->target, right);
}

// Moves the camera position closer/farther to/from the camera target. From rcamera.h
void CameraMoveToTarget(Camera* camera, float delta)
{
	float distance = Vector3Distance(camera->position, camera->target);

	// Apply delta
	distance += delta;

	// Distance must be greater than 0
	if (distance <= 0) distance = 0.001f;

	// Set new distance by moving the position along the forward vector
	Vector3 forward = GetCameraForward(camera);
	camera->position = Vector3Add(camera->target, Vector3Scale(forward, -distance));
}

// Update the camera with new values. Inputs the new player position and the change in player position
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
	if (cameraMode == CAMERA_FIRST_PERSON) {
		// Move camera to the position of target, and reset target
		camera.position = playerPos;
		camera.target = Vector3Add(camera.target, changePos);
		UpdateCameraPro(&camera,
			{ 0, 0, 0 },
			Vector3{ GetMouseDelta().x * BASE_CAMERA_SPEED, GetMouseDelta().y * BASE_CAMERA_SPEED, 0.0f },
			0.0f
		);
	}
		
	if (cameraMode == CAMERA_THIRD_PERSON) {
		Vector2 mousePosDelta = GetMouseDelta();		
		// pitch / yaw camera around target based on mouse movement
		CameraYaw(&camera, -mousePosDelta.x * CAMERA_MOUSE_MOVE_SENSITIVITY, true);
		CameraPitch(&camera, -mousePosDelta.y * CAMERA_MOUSE_MOVE_SENSITIVITY, true, true);

		// Update camera position & target to reflect player movement
		camera.target = playerPos;
		camera.position = Vector3Add(camera.position, changePos);

		// Zoom camera in and out
		CameraMoveToTarget(&camera, -GetMouseWheelMove());
	}

	if (cameraMode == CAMERA_FREE) {
		UpdateCamera(&camera, cameraMode);
	}
}

Camera CameraController::getCamera() {
	return camera;
}

int CameraController::getCameraMode() {
	return cameraMode;
}

CameraController::~CameraController() {

}