#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include "cTexture.h"
#include "cGameObject.h"
#include "cGUI.h"

class cGame
{
private:
	cGame();
	static cGame *singleton;

	cGUI *gui;
	bool bPaused;
	bool bMenu;
	cTexture texture;
	vector<cGameObject*> gameObjects;
	vector<cGameObject*> objctsToDelete;

	bool PerPixelCollision(cGameObject* g1, cGameObject* g2);

public:
	~cGame();
	static cGame* Get() { if (!singleton) singleton = new cGame(); return singleton; }

	void Update(float delta);
	void CollisionUpdate();
	void Render();
	void AddGameObject(cGameObject *obj) { gameObjects.push_back(obj); }
	cGameObject* ClickedOn(glm::vec2 pos);
};