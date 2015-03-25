#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"

#define GUI_SCREENS 9
enum class Screen { Main, Instructions, Highscores, Options, GameOverlay, Pause, PauseOptions, Death, Global };

class cGUI
{
private:
	glm::vec2 windowSize;
	Screen currentMenu = Screen::Main;
	vector<cGUIElement *> menus[GUI_SCREENS];
	float timer = 0;
	int frames = 0;
	int fps = 0;
	int finalScore = 0;

	cTexture *tutorialTexture;

	void SetUp();
	void ToggleBackground();
	void ToggleVolume();
	void UpdateVolumeBtn();
	void UpdateBackgroundBtn();
	void CleanUp();
	void CleanUp(Screen menu);

	//menu set up methods
	void SetUpMain();
	void SetUpInstructions();
	void SetUpHightscores();
	void SetUpOptions();
	void SetUpGameOverlay();
	void SetUpPause();
	void SetUpPauseOptions();
	void SetUpDeath();
	void SetUpGlobalDataOverlay();

public:
	cGUI(glm::vec2 pWindowSize, cTexture *pTutorialTexture);
	~cGUI();

	void UpdateSize(glm::vec2 newSize);
	void SetMenu(Screen menu);

	void Update(float delta);
	void Render(glm::vec2 offset);
	void SetFinalScore(int score) { finalScore = score; }
};

