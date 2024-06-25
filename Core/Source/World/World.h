// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// 
// --------------------------------------------------
#pragma once

#include <raylib.h>
#include <vector>
#include <memory>

#include "Core/GameObject.h"
#include "Physics/PhysicsWorld.h"
#include "Core/GameEntity.h"
#include "Core/CommandDispatcher.h"

// Some basic physics colors for debugging
const std::vector<Color> physicsColors{
			MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, RED, GOLD, LIME, BLUE, VIOLET, PINK, YELLOW, GREEN, SKYBLUE, PURPLE
};

class World : public GameEntity {
private:
	PhysicsWorld physicsWorld;
	CommandDispatcher& dispatcher;
	bool playerSpawned;
	int idCounter;
public:
	GameObject localPlayerObject;
	std::vector<GameObject> gameObjects;
	std::vector<GameObject> spawnedGameObjects;
	
	World(CommandDispatcher& dispatcher);
	~World();
	
	void populate();
	
	void spawnObject(GameObject go, Vector3 position={0, 0, 0}, Vector3 rotation = {0, 0, 0}, Vector3 size = {0, 0, 0}); // place an object in the world
	void despawnObject(GameObject& go); // remove an object from the world
	GameObject createObject(Vector3 size, float mass, PhysicsInteractionType type, PhysicsShape shape, Color color, bool visibility, bool replicates); // create a GameObject
	void destroyObject(GameObject& go); // destroy a GameObject
	
	void createLocalPlayerObject();
	void spawnLocalPlayer();
	void despawnLocalPlayer();
	// Note: do not need to destroy as will be handled by scope of world
	
	void clearSpawned();
	void clearWorld();
	
	void syncToPhysics();
	void printDebug();
	
	void receiveCommand(Command command);
	
	void update(float deltaTime);
	void render(bool drawDebug = false);
};