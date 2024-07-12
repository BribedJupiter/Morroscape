// --------------------------------------------------
// 
// Morroscape
// Created 1/6/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <raylib.h>

const float BASE_CAMERA_SPEED = 0.05f;

class CameraController {
private:
	Camera camera;
	float cameraSpeed;
	int cameraMode;
public:
	CameraController();
	~CameraController();
	Camera getCamera();
	void setCameraMode(CameraMode mode);
	int getCameraMode();
	void update(float deltaTime, Vector3 playerPos, Vector3 changePos);
};