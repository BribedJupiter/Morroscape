// --------------------------------------------------
// 
// Morroscape
// Created 1/4/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <raylib.h>
#include <vector>
#include <memory>
#include <iostream>

#include "World.h"

const int MAX_COLUMNS = 100;

World::World(CommandDispatcher& dispatcher) : physicsWorld(PhysicsWorld()), dispatcher(dispatcher), playerSpawned(false), idCounter(0) {
	createLocalPlayerObject();
}

void World::populate() {
	// spawn the columns, balls, and blocks
	// create object -> spawn object (generate physics object and add to physics world)

	// Create walls
	spawnObject(createObject({ 32, 0.5, 32 }, 0.0, STATIC, BOX, WHITE, true, true), {0, -0.25, 0}, {0, 0, 0}, {64, 0.5, 64});
	spawnObject(createObject({ 1, 5, 32 }, 0.0, STATIC, BOX, BLUE, true, true), { -16.0f, 2.5f, 0.0f }, { 0, 0, 0 }, { 1, 5, 32 });
	spawnObject(createObject({ 1, 5, 32 }, 0.0, STATIC, BOX, LIME, true, true), { 16.0f, 2.5f, 0.0f }, { 0, 0, 0 }, { 1, 5, 32 });
	spawnObject(createObject({ 32, 5, 1 }, 0.0, STATIC, BOX, GOLD, true, true), { 0.0f, 2.5f, 16.0f }, { 0, 0, 0 }, { 32, 5, 1 });

	// Create debug cylinders
	spawnObject(createObject({ 1, 1, 1 }, 0.0, STATIC, CAPSULE, RED, true, true), { 20, 3, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	spawnObject(createObject({ 1, 3, 1 }, 0.0, STATIC, CAPSULE, BLUE, true, true), { 20, 3, 5 }, { 0, 0, 0 }, { 1, 3, 1 });
	spawnObject(createObject({ 1, 3, 1 }, 0.0, STATIC, CAPSULE, WHITE, true, true), { 20, 3, 7 }, { 0, 0, 0 }, { 1, 4, 1 });
	spawnObject(createObject({ 3, 1, 1 }, 0.0, STATIC, CAPSULE, GOLD, true, true), { 20, 3, 10 }, { 0, 0, 0 }, { 3, 2, 1 });
	spawnObject(createObject({ 1, 1, 3 }, 0.0, STATIC, CAPSULE, PURPLE, true, true), { 20, 3, 15 }, { 0, 0, 0 }, { 1, 6, 3 });


	// Columns
	float heights[MAX_COLUMNS] = {0};
	Vector3 positions[MAX_COLUMNS] = { 0 };
	Color colors[MAX_COLUMNS] = { 0 };

	for (int i = 0; i < MAX_COLUMNS; i++) {
		heights[i] = float(GetRandomValue(1, 12));
		positions[i] = Vector3{ float(GetRandomValue(-15, 15)), heights[i] / 2.0f, float(GetRandomValue(-15, 15)) };
		colors[i] = Color{ static_cast<unsigned char>(GetRandomValue(20, 255)), static_cast<unsigned char>(GetRandomValue(10, 55)), 30, 255 };
	}
	for (int i = 0; i < MAX_COLUMNS; i++) {
		int height = GetRandomValue(1, 10);
		spawnObject(createObject({ 1, float(height), 1 }, 0.0f, STATIC, BOX, physicsColors[GetRandomValue(0, physicsColors.size() - 1)], true, true), positions[i], {0, 0, 0}, { 1, float(height), 1 });
	}

	// Add in the rest of the physics objects
	for (int i = 0; i < 30; i++) {
		Vector3 size = {float(GetRandomValue(1, 3)), float(GetRandomValue(1, 3)), float(GetRandomValue(1, 3))};
		Vector3 pos = {float(GetRandomValue(-5, 5)), float(GetRandomValue(10, 15)), float(GetRandomValue(-5, 5))};
		Vector3 rot = {float(GetRandomValue(-3, 3)), float(GetRandomValue(-3, 3)), float(GetRandomValue(-3, 3))};
		spawnObject(createObject(size, 1.0, DYNAMIC, PhysicsShape(GetRandomValue(0, 2)), physicsColors[GetRandomValue(0, physicsColors.size() - 1)], true, true), pos, rot, size);
	}

	std::cout << "[World] World populated!" << std::endl;
	printDebug();
}

// Copy a game object, generate a physics object for it, then add to physicsWorld and the spawnedGameObjects vector
void World::spawnObject(GameObject go, Vector3 position, Vector3 rotation, Vector3 size) {
	spawnedGameObjects.push_back(go); // add a copy of the GameObject to the spawnedObjects
	GameObject& goToSpawn = spawnedGameObjects.back(); // get a reference to that new copy in the vector
	goToSpawn.generatePhysicsComponent(position, rotation, size); // generate its physicsComponent
	physicsWorld.addToWorld(goToSpawn.getPhysicsComponent()); // add this copy to the physicsWorld
	std::cout << "[World::spawnObject] gameObject was spawned with ID: " << goToSpawn.id << std::endl;
}

// Remove a GameObject from the world - Note: should input a reference to a game object IN spawnedGameObject, does not affect anything in gameObjects
void World::despawnObject(GameObject& go) {
	auto goSpawnedIndex = std::find(spawnedGameObjects.begin(), spawnedGameObjects.end(), go);
	if (!(goSpawnedIndex != spawnedGameObjects.end())) {
		// go is not in vector
		std::cout << "[World] could not find gameObject in World::spawnedGameObjects with ID: " << go.id << std::endl;
		return;
	}
	// remove the physics object from the world
	// delete the physics object
	GameObject& goToDespawn = *goSpawnedIndex; // get a reference to the game object we found
	physicsWorld.removeFromWorld(goToDespawn.getPhysicsComponent()); // remove it from the physics world
	goToDespawn.removePhysicsComponent();
	std::cout << "[World] gameObject was despawned with ID: " << goToDespawn.id << std::endl;
	spawnedGameObjects.erase(goSpawnedIndex); // erase it from the spawned list & destructor removes physics object
}

// Create a GameObject, add it to the gameObjects vector, return a copy
GameObject World::createObject(Vector3 size, float mass, PhysicsInteractionType type, PhysicsShape shape, Color color, bool visibility, bool replicates) {
	GameObject go = GameObject(idCounter, size, mass, type, shape, color, visibility, replicates); // generate a local GameObject
	idCounter++;
	gameObjects.push_back(go); // add a copy of the GameObject to the GameObjects vector
	std::cout << "[World] gameObject was created with ID: " << gameObjects.back().id << std::endl;
	return gameObjects.back(); // return a copy of the generated GameObject
}

// Destory and delete a GameObject from gameObjects - Note: does not despawn the object or affect spawnedGameObjects
void World::destroyObject(GameObject& go) {
	auto go_index = std::find(gameObjects.begin(), gameObjects.end(), go);
	if (!(go_index != gameObjects.end())) {
		// go is not in vector
		std::cout << "[World] could not find gameObject in World::gameObjects with ID: " << go.id << std::endl;
		return;
	}
	int gID = go_index->id;
	gameObjects.erase(go_index);
	std::cout << "[World] gameObject was deleted with ID: " << gID << std::endl;
}

void World::createLocalPlayerObject() {
	GameObject go = GameObject(idCounter, { 4, 4, 4 }, 5.0, DYNAMIC, BOX, WHITE, true, true); // generate a local GameObject
	idCounter++;
	localPlayerObject = go; // set localPlayerObject to a copy of the new game object
	std::cout << "[World] localPlayerObject was created with ID: " << localPlayerObject.id << std::endl;
}

void World::spawnLocalPlayer() {
	localPlayerObject.generatePhysicsComponent({0.0, 3.0, 0.0}, {0.0, 0.0, 0.0}, {2.0, 2.0, 2.0}); // generate its physicsComponent
	PhysicsComponent& physicsComponentRef = localPlayerObject.getPhysicsComponent(); // get an alias to the new physics component
	physicsWorld.addToWorld(physicsComponentRef); // add this copy to the physicsWorld
	// Set proper player settings
	physicsComponentRef.body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f)); // lock player rotation due to physics
	playerSpawned = true;
	dispatcher.dispatchCommand({ this->name, "Local Player Controller", "SET PHYSICS COMPONENT", localPlayerObject.getRawPhysicsComponent() });
	std::cout << "[World] localPlayerObject was spawned with ID: " << localPlayerObject.id << std::endl;
}

void World::despawnLocalPlayer() {
	if (playerSpawned) {
		dispatcher.dispatchCommand({ this->name, "Local Player Controller", "UNSET PHYSICS COMPONENT", nullptr });
		physicsWorld.removeFromWorld(localPlayerObject.getPhysicsComponent());
		localPlayerObject.removePhysicsComponent();
		playerSpawned = false;
		std::cout << "[World] localPlayerObject was despawned with ID: " << localPlayerObject.id << std::endl;
	}
	else 
		std::cout << "[World] localPlayerObject is not spawned with ID: " << localPlayerObject.id << std::endl;
}

void World::clearSpawned() {
	std::cout << "[World] spawnedGameObjects has " << spawnedGameObjects.size() << " elements" << std::endl;
	int iter = 0;
	for (int i = spawnedGameObjects.size() - 1; i >= 0; i--) {
		despawnObject(spawnedGameObjects[i]);
		iter++;
	}
	despawnLocalPlayer();
	std::cout << "[World] spawned cleared with iterations: " << iter << std::endl;
	printDebug();
	spawnedGameObjects.clear();
}

void World::clearWorld() {
	clearSpawned();
	std::cout << "[World] gameObjects has " << gameObjects.size() << " elements" << std::endl;
	int iter = 0;
	for (int i = gameObjects.size() - 1; i >= 0; i--) {
		destroyObject(gameObjects[i]);
		iter++;
	}
	std::cout << "[World] game objects cleared with iterations: " << iter << std::endl;
	printDebug();
	gameObjects.clear();
}

void World::syncToPhysics() {

}

void World::printDebug() {
	std::cout << "----------" << std::endl;
	std::cout << "PRINTING gameObjects" << std::endl;
	for (auto& go : gameObjects) {
		std::cout << "[ID] " << go.id << std::endl;
	}
	std::cout << "PRINTING spawnedGameObjects" << std::endl;
	for (auto& sgo : spawnedGameObjects) {
		std::cout << "[ID] " << sgo.id << std::endl;
	}
	std::cout << "----------" << std::endl;
}

void World::receiveCommand(Command command) {
	if (command.message == "SPAWN PLAYER") {
		if (playerSpawned) {
			despawnLocalPlayer();
		}
		spawnLocalPlayer();
		std::cout << "[" << this->name << "] Spawned player" << std::endl;
	}
	if (command.message == "DESPAWN PLAYER") {
		despawnLocalPlayer();
		std::cout << "[" << this->name << "] Despawned player" << std::endl;
	}
	if (command.message == "REQUEST PLAYER") {
		// Unnecessary - local player controller has physics component and can tell if spawned by if physicsComponent is nullptr or not
		//if (playerSpawned)
		//	dispatcher.dispatchCommand({ this->name, "Local Player Controller", "SEND PLAYER SPAWNED", &localPlayerObject });
		//else 
		//	dispatcher.dispatchCommand({ this->name, "Local Player Controller", "SEND PLAYER DESPAWNED", &localPlayerObject });
	}
	else {
		//std::cerr << "[" << this->name << "][ERROR] received false command " << command.message << " from " << command.sender << " at address " << command.address << std::endl;
	}
}

void World::update(float deltaTime) {
	float targetPhysicsFPS = (deltaTime < 1 && deltaTime > 0) ? deltaTime : 1.0 / PHYSICS_FRAME_TARGET; // Stop physics simulations from being run with crazy long time steps if deltaTime is too big, or if it is 0
	physicsWorld.update(targetPhysicsFPS);
}

void World::render(bool drawDebug) {
	for (GameObject object : spawnedGameObjects) {
		if (object.visible)
		object.render();
	}
	if (playerSpawned) {
		localPlayerObject.render();
	}
	physicsWorld.render();
}

World::~World() {
	// delete all game objects
	clearWorld(); // autocalls clearSpawned()
}