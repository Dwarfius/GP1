#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include "cTexture.h"
#include "cGameObject.h"

class cGame
{
private:
	bool bPaused;
	bool bMenu;

	cTexture texture;
	vector<cGameObject*> gameObjects;

	bool PerPixelCollision(cGameObject* g1, cGameObject* g2);

public:
	cGame();
	~cGame();

	void Update(float delta);
	void CollisionUpdate();
	void Render();
};