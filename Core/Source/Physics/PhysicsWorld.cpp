// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// Aided by https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
// 
// --------------------------------------------------
#include <bullet/btBulletDynamicsCommon.h>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "PhysicsWorld.h"

// Initialize the physics world
PhysicsWorld::PhysicsWorld() 
	: collisionConfiguration(new btDefaultCollisionConfiguration()), dispatcher(new btCollisionDispatcher(collisionConfiguration)),
	overlappingPairCache(new btDbvtBroadphase()), solver(new btSequentialImpulseConstraintSolver()), 
	dynamicsWorld(new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration)),
	debugDrawer(PhysicsDebugDrawer())
{
	// Setup world
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debugDrawer);
}

// Check for a collision between two physics objects
bool PhysicsWorld::checkPhysicsComponentCollision(PhysicsComponent& po1, PhysicsComponent& po2) {
	SimulationContactResultCallback collisionCallback;
	btCollisionObject* co1 = po1.getCollisionObject();
	btCollisionObject* co2 = po2.getCollisionObject();
	dynamicsWorld->contactPairTest(co1, co2, collisionCallback);
	return collisionCallback.bCollision;
}

// add a physics object to the world
void PhysicsWorld::addToWorld(PhysicsComponent& po) {
	collisionShapes.push_back(po.collisionShape);
	dynamicsWorld->addRigidBody(po.body);
	std::cout << "[PhysicsWorld] PhysicsComponent was added to world" << std::endl;
}

// remove from physicsWorld, delete collisionShape, delete rigidBody, DOES NOT DELETE OBJECT
// TODO: Check if in world?
void PhysicsWorld::removeFromWorld(PhysicsComponent& po) {
	collisionShapes.remove(po.collisionShape);
	dynamicsWorld->removeCollisionObject(po.body);
	delete po.body->getMotionState();
	delete po.body;
	delete po.collisionShape;
	std::cout << "[PhysicsWorld] PhysicsComponent was removed from world" << std::endl;
}

void PhysicsWorld::update(btScalar stepTime) {
	dynamicsWorld->stepSimulation(btScalar(stepTime), 0); // Update physics
	// Note: When maxSubSteps was 10, I noticed a huge desync between where the player was rendered vs where the physics debug 
	// drawing showed. Setting it to zero fixed this issue. 
}

void PhysicsWorld::render() {
	dynamicsWorld->debugDrawWorld();
}

PhysicsWorld::~PhysicsWorld() {
	// Physics cleanup - should be in reverse order of creation / initialization
	// remove rigidbodies from dynamicsWorld and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	// delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++) {
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] - 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();
}