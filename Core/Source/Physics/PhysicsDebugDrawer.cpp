// --------------------------------------------------
// 
// Morroscape
// Created 6/24/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <raylib.h>
#include <iostream>

#include "PhysicsDebugDrawer.h"

PhysicsDebugDrawer::PhysicsDebugDrawer() {

}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	DrawLine3D(
		{ float(from.getX()), float(from.getY()), float(from.getZ()) },
		{ float(to.getX()), float(to.getY()), float(to.getZ()) },
		GREEN
	);
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
	DrawPoint3D(
		{ float(PointOnB.getX()), float(PointOnB.getY()), float(PointOnB.getZ()) },
		LIME
	);
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
	std::cout << warningString << std::endl;
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
	
}


void PhysicsDebugDrawer::setDebugMode(int debugMode) {
	iDebugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const {
	return iDebugMode;
}

PhysicsDebugDrawer::~PhysicsDebugDrawer() {

}