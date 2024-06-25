// --------------------------------------------------
// 
// Morroscape
// Created 6/22/2024
// Jack Bauer
// Aided by https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
// And by https://gist.github.com/samfromcadott/58a59967cde3f51a51265def0ffd837a
// 
// --------------------------------------------------
#include <iostream>
#include <raylib.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(
	Vector3 pos, Vector3 rot, Vector3 size, float mass, PhysicsInteractionType type, PhysicsShape shape, Color color) 
	: shape(shape), collisionShape(nullptr), body(nullptr),  color(color)
{
	// 1. Set location, rotation, mass, and size
	// 2. Build the model and determine the collisionShape (the representation of the object in the world)
	// 3. Caclulate appropriate local intertia for dynamic objects

	// 1 - set up transform and mass
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	transform.setRotation(btQuaternion(btScalar(rot.z), btScalar(rot.y), btScalar(rot.x)));

	// 2 - model & collision shape
	if (shape == CAPSULE) {
		collisionShape = new btCylinderShape(btVector3(btScalar(size.x / 2.0), btScalar(size.y / 2.0), btScalar(size.z / 2.0)));
		model = LoadModelFromMesh(GenMeshCylinder(size.x / 2.0, size.y, 16));
	}
	if (shape == BOX) {
		collisionShape = new btBoxShape(btVector3(btScalar(size.x / 2.0), btScalar(size.y / 2.0), btScalar(size.z / 2.0)));
		model = LoadModelFromMesh(GenMeshCube(size.x, size.y, size.z));
	}
	if (shape == SPHERE) {
		collisionShape = new btSphereShape(size.x);
		model = LoadModelFromMesh(GenMeshSphere(size.x, 8, 16));
	}

	// 3 - for dynamic objects calculate local inertia
	btVector3 localInertia(0, 0, 0);
	if (collisionShape != nullptr && (type == DYNAMIC || mass != 0.0)) {
		collisionShape->calculateLocalInertia(btScalar(mass), localInertia);
	}

	// Finish setting up the motion state and rigidbody
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbConstructor(btScalar(mass), motionState, collisionShape, localInertia);
	body = new btRigidBody(rbConstructor);
}

void PhysicsComponent::render() {
	// 1. Get transform information - position and rotation
	// 2. Then use that to render the model appropriately

	// 1 - setup a render transform
	btTransform renderTransform;

	if (body && body->getMotionState()) {
		body->getMotionState()->getWorldTransform(renderTransform); // retrieve transform information and apply to variable
	}
	else {
		throw std::runtime_error("Attempting to render object without motion state");
	}

	btQuaternion renderQuat = renderTransform.getRotation();
	float renderAngle = float(renderQuat.getAngle()) * 57.2958; // Get quaternion angle and multiply to convert to degrees from radian

	// We need to adjust the render since bullet has the origin at center of mass, and raylib has the origin at the bottom for some reason
	if (shape == CAPSULE) {
		btTransform lt;
		lt.mult(renderTransform, renderTransform.inverse()); // get the local transform
		btVector3 centerPos = lt.getOrigin(); // get the center of the cylinder

		btCylinderShape* cylinder = static_cast<btCylinderShape*> (collisionShape); // get a cylinder object so we can get the half extents
		btVector3 halves = cylinder->getHalfExtentsWithMargin(); 

		lt.getOrigin().setY(centerPos.getY() - halves.getY()); // set the new origin of the render transform to center - 1/2 height
		renderTransform.mult(renderTransform, lt); // apply the adjusted local transform to the render transform
	}

	Vector3 renderPos = {
		float(renderTransform.getOrigin().getX()),
		float(renderTransform.getOrigin().getY()),
		float(renderTransform.getOrigin().getZ())
	};
	

	Vector3 renderAxis = {
		float(renderQuat.getAxis().getX()),
		float(renderQuat.getAxis().getY()),
		float(renderQuat.getAxis().getZ())
	};

	// 2 - render the model
	DrawModelEx(model, renderPos, renderAxis, renderAngle, { 1, 1, 1 }, color);
	DrawModelWiresEx(model, renderPos, renderAxis, renderAngle, { 1, 1, 1 }, 
		{ (unsigned char)(color.r / 2), (unsigned char)(color.g / 2), (unsigned char)(color.b / 2), color.a });
}

Vector3 PhysicsComponent::getPosition() {
	btTransform transform;
	// Get body transforms
	if (body && body->getMotionState()) {
		body->getMotionState()->getWorldTransform(transform);
	}
	else {
		throw std::runtime_error("Rigidbody not found");
	}

	// Get the position
	Vector3 position = {
		float(transform.getOrigin().getX()),
		float(transform.getOrigin().getY()),
		float(transform.getOrigin().getZ())

	};
	return position;
}

btCollisionObject* PhysicsComponent::getCollisionObject() {
	btCollisionObject* obj = static_cast<btCollisionObject*>(body);
	return obj;
}

btCollisionShape* PhysicsComponent::getCollisionShape() {
	return collisionShape;
}

Model PhysicsComponent::getModel() {
	return model;
}

btRigidBody* PhysicsComponent::getRigidBody() {
	return body;
}

PhysicsComponent::~PhysicsComponent() {
	// body, motion state, collider shape are deleted when removed from physics world
	UnloadModel(model);
}
