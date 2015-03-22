#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"

#define GUI_SCREENS 8
enum class Screen { Main, Instructions, Options, GameOverlay, Pause, PauseOptions, Death, Global };

class cGUI
{
private:
	glm::vec2 windowSize;
	Screen currentMenu = Screen::Main;
	vector<cGUIElement *> menus[GUI_SCREENS];
	float timer = 0;
	int frames = 0;
	int fps = 0;

	cTexture *tutorialTexture;

	void SetUp();
	void ToggleBackground();
	void ToggleVolume();
	void CleanUp();

public:
	cGUI(glm::vec2 pWindowSize, cTexture *pTutorialTexture);
	~cGUI();

	void UpdateSize(glm::vec2 newSize);
	void SetMenu(Screen menu) { currentMenu = menu; }

	void Update(float delta);
	void Render(glm::vec2 offset);
	void SetFinalScore(int score);
};

