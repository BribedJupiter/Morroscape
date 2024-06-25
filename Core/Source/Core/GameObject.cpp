// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <iostream>
#include <raylib.h>
#include "GameObject.h"

GameObject::GameObject(int id, Vector3 initSize, float mass, PhysicsInteractionType type, PhysicsShape shape,  Color color, bool visibility, bool replicate)
	: physicsComponent(nullptr), replicates(replicate), physicsDetails(PhysicsDetails()), visible(visibility), id(id)
{
	physicsDetails.size = initSize;
	physicsDetails.shape = shape;
	physicsDetails.type = type;
	physicsDetails.mass = mass;
	physicsDetails.color = color;
}

GameObject::GameObject(const GameObject& other) {
	if (other.physicsComponent != nullptr) {
		physicsComponent = other.physicsComponent;
	}
	else {
		physicsComponent = nullptr;
	}
	
	physicsDetails.size = other.physicsDetails.size;
	physicsDetails.mass = other.physicsDetails.mass;
	physicsDetails.type = other.physicsDetails.type;
	physicsDetails.shape = other.physicsDetails.shape;
	physicsDetails.color = other.physicsDetails.color;
	replicates = other.replicates;
	visible = other.visible;
	id = other.id;
}

void GameObject::render() {
	physicsComponent->render();
}

// Generate a PhysicsComponent and assign it to physicsComponent
void GameObject::generatePhysicsComponent(Vector3 position, Vector3 rotation, Vector3 size) {
	physicsComponent = new PhysicsComponent(
		position, rotation, size, physicsDetails.mass, physicsDetails.type, physicsDetails.shape,
		physicsDetails.color
	);
	std::cout << "[GameObject] PhysicsComponent generated for ID: " << id << std::endl;
}

Vector3 GameObject::getPosition() {
	return physicsComponent->getPosition();
}

PhysicsComponent& GameObject::getPhysicsComponent() {
	return *physicsComponent; // return the dereferenced physicsComponent
}

void GameObject::removePhysicsComponent() {
	delete physicsComponent;
	std::cout << "[GameObject] PhysicsComponent deleted for ID: " << id << std::endl;
}

GameObject::~GameObject() {
	// world will delete physics components
}