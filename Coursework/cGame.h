#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include "cTexture.h"
#include "cGameObject.h"

class cGame
{
private:
	HWND hWnd;
	glm::vec2 mousePos;

	bool bPaused;
	bool bMenu;

	cTexture texture;
	vector<cGameObject*> gameObjects;

public:
	cGame();
	~cGame();

	void SetHWND(HWND phWnd) { hWnd = phWnd; }

	void Update(float delta);
	void Render();
};