// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// Aided by https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
// 
// --------------------------------------------------
#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <raylib.h>
#include <vector>
#include <memory>

#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"
#include "Core/GameEntity.h"

const float PHYSICS_FRAME_TARGET = 120.0f;

// For use in physics contact tests
struct SimulationContactResultCallback : public btCollisionWorld::ContactResultCallback {
	bool bCollision = false;
	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		bCollision = true;
		return 0;
	}
};

class PhysicsWorld : public GameEntity {
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	SimulationContactResultCallback collisionCallback;
	PhysicsDebugDrawer debugDrawer;
	
public:
	PhysicsWorld(); // Initialize the physics world
	~PhysicsWorld();
	bool checkPhysicsComponentCollision(PhysicsComponent& po1, PhysicsComponent& po2); // check for collision between two game objects
	void addToWorld(PhysicsComponent& po); // add a physics object to the world
	void removeFromWorld(PhysicsComponent& po); // remove a game object from the physics world
	void update(btScalar stepTime);
	void render();
};