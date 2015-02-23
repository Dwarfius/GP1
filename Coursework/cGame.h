#pragma once

#include "GameConstants.h"
class cGame
{
private:
	HWND hWnd;
	glm::vec2 mousePos;

	bool bPaused;
	bool bMenu;

	cTexture texture;
	cSprite sprite;
	cGameObject gameObject;

public:
	cGame();
	~cGame();

	void SetHWND(HWND phWnd) { hWnd = phWnd; }

	void HandleInput(WPARAM key);
	void Update(float delta);
	void Render();
};