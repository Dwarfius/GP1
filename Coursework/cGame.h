#pragma once

#include "GameConstants.h"
class cGame
{
private:
	bool bPaused;
	bool bMenu;

	cTexture texture;
	cSprite sprite;
public:
	cGame();
	~cGame();

	void HandleInput(WPARAM key);
	void Update(float delta);
	void Render();
};