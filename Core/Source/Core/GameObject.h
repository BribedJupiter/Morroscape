// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <memory>

#include "Physics/PhysicsComponent.h"
#include "GameEntity.h"

struct PhysicsDetails {
	Vector3 size;
	float mass;
	PhysicsInteractionType type;
	PhysicsShape shape;
	Color color;
};

class PhysicsComponent;

class GameObject : public GameEntity {
private:
	PhysicsComponent* physicsComponent;
	bool replicates;
public:
	PhysicsDetails physicsDetails;
	bool visible;
	int id;

	GameObject(
		int id = 0, 
		Vector3 initSize={1, 1, 1}, 
		float mass = 0.0f,
		PhysicsInteractionType type = STATIC,
		PhysicsShape shape = BOX,
		Color color = RED, 
		bool visibility = true, 
		bool replicate = true
	); // initialize the game object
	
	GameObject(const GameObject& copy); // game object copy constructor
	
	void render();
	void generatePhysicsComponent(Vector3 position, Vector3 rotation, Vector3 size);
	void removePhysicsComponent();

	Vector3 getPosition(); // return the position of the object
	PhysicsComponent& getPhysicsComponent(); // Return a reference to the physics component
	PhysicsComponent* getRawPhysicsComponent(); // Return the physics component pointer itself

	~GameObject();

	bool operator==(const GameObject& other) const {
		if (physicsComponent != other.physicsComponent) {
			return false;
		}
		if (!((physicsDetails.size.x == other.physicsDetails.size.x)
			&& (physicsDetails.size.y == other.physicsDetails.size.y)
			&& (physicsDetails.size.z == other.physicsDetails.size.z))) {
			return false;
		}
		if (visible != other.visible) {
			return false;
		}
		if (replicates != other.replicates) {
			return false;
		}
		return true;
	}
};