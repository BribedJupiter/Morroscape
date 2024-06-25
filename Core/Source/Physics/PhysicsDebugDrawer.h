// --------------------------------------------------
// 
// Morroscape
// Created 6/24/2024
// Jack Bauer
// 
// --------------------------------------------------
#include <bullet/LinearMath/btIDebugDraw.h>
#include <raylib.h>

class PhysicsDebugDrawer : public btIDebugDraw {
private:
	int iDebugMode = 0;
public:
	PhysicsDebugDrawer();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;

	virtual void reportErrorWarning(const char* warningString) override;

	virtual void draw3dText(const btVector3& location, const char* textString) override;
	
	virtual void setDebugMode(int debugMode) override;

	virtual int getDebugMode() const override;

	~PhysicsDebugDrawer();
};