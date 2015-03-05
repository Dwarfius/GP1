#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include "cTexture.h"
#include "cGameObject.h"

class cGame
{
private:
	cGame();
	static cGame *singleton;
	bool bPaused;
	bool bMenu;

	cTexture texture;
	vector<cGameObject*> gameObjects;

	bool PerPixelCollision(cGameObject* g1, cGameObject* g2);

public:
	~cGame();

	void Update(float delta);
	void CollisionUpdate();
	void Render();
	void AddGameObject(cGameObject *obj) { gameObjects.push_back(obj); }
	static cGame* Get() { if (!singleton) singleton = new cGame(); return singleton; }
};