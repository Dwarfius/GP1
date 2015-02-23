#pragma once

#include "GameConstants.h"
class cGame
{
private:
	HWND hWnd;

	bool bPaused;
	bool bMenu;

	cTexture texture;
	cSprite sprite;
public:
	cGame();
	~cGame();

	void SetHWND(HWND phWnd) { hWnd = phWnd; }

	void HandleInput(WPARAM key);
	void Update(float delta);
	void Render();
};