#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"
#include "cGUIButton.h"

#define COLOR_NORMAL glm::vec4(0, 0, 1, 1)
#define COLOR_HIGHL glm::vec4(0.5f, 0.5f, 1, 1)

#define GUI_SCREENS 10
enum class Screen { Main, Instructions, Highscores, Options, GameOverlay, Upgrade, Pause, PauseOptions, Death, Global };

class cGUI
{
private:
	glm::vec2 windowSize;
	Screen currentMenu = Screen::Main;
	vector<cGUIElement *> menus[GUI_SCREENS];
	vector<cGUIButton *> btns[GUI_SCREENS];
	float timer = 0;
	int frames = 0;
	int fps = 0;
	int finalScore = 0;
	int activeBtn = 0;

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
	void SetUpUpgrade();
	void SetUpPause();
	void SetUpPauseOptions();
	void SetUpDeath();
	void SetUpGlobalDataOverlay();

	string GetUpgradeLabelText();

public:
	cGUI(glm::vec2 pWindowSize, cTexture *pTutorialTexture);
	~cGUI();

	void UpdateSize(glm::vec2 newSize);
	void SetMenu(Screen menu);

	void Update(float delta);
	void Render(glm::vec2 offset);
	void SetFinalScore(int score) { finalScore = score; }
};

