// --------------------------------------------------
// 
// Morroscape
// Created 1/2/2024
// Jack Bauer
// Aided by https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
// And by https://gist.github.com/samfromcadott/58a59967cde3f51a51265def0ffd837a
// 
// --------------------------------------------------
#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <raylib.h>

#include "Core/GameEntity.h"

enum PhysicsInteractionType {
	DYNAMIC,
	STATIC
};

enum PhysicsShape {
	CAPSULE,
	BOX,
	SPHERE
};

class PhysicsComponent : public GameEntity {
private:
	Model model;
	Color color;
	PhysicsShape shape;

public:
	btCollisionShape* collisionShape;
	btRigidBody* body;

	PhysicsComponent(
		Vector3 position = {0, 0, 0},
		Vector3 rotation = {0, 0, 0},
		Vector3 size = {1, 1, 1},
		float mass = 0,
		PhysicsInteractionType = STATIC,
		PhysicsShape shape = BOX,
		Color color = PURPLE
	);

	void render();
	
	Vector3 getPosition();
	btCollisionObject* getCollisionObject();
	btCollisionShape* getCollisionShape();
	Model getModel();
	btRigidBody* getRigidBody();

	~PhysicsComponent();
};
