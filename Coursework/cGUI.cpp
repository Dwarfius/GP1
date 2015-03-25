#include "cGUI.h"
#include "GameConstants.h"
#include "cGUILabel.h"
#include "cGUIButton.h"
#include "cGUIProgressBar.h"
#include "cGame.h"
#include "cInput.h"
#include "cSettings.h"

#pragma warning(disable: 4018 4244)

cGUI::cGUI(glm::vec2 pWindowSize, cTexture *pTutorialTexture)
{
	windowSize = pWindowSize;
	tutorialTexture = pTutorialTexture;
	SetUp();
}

cGUI::~cGUI()
{
	CleanUp();
}

void cGUI::UpdateSize(glm::vec2 newSize)
{
	windowSize = newSize;
	CleanUp();
	SetUp();
}

void cGUI::SetUp()
{
	SetUpMain();
	SetUpInstructions();
	SetUpOptions();
	SetUpGameOverlay();
	SetUpPause();
	SetUpPauseOptions();
	SetUpGlobalDataOverlay();
}

void cGUI::Update(float delta)
{
	//per menu elements
	switch (currentMenu)
	{
	case Screen::GameOverlay:
	{
		cPlayer *player = cGame::Get()->GetPlayer();
		float health = player->GetHealth();
		float max = player->GetMaxHealth();
		cGUIProgressBar *bar = (cGUIProgressBar*)menus[(int)Screen::GameOverlay][0];
		bar->SetPercentage(health / max);
		bar->SetText(to_string((int)health) + "/" + to_string((int)max));

		cGUILabel *lbl = (cGUILabel*)menus[(int)Screen::GameOverlay][1];
		lbl->SetText("Score: " + to_string(cGame::Get()->GetScore()));

		if (cInput::GetKeyDown(27))
		{
			SetMenu(Screen::Pause);
			cGame::Get()->SetPaused(true);
		}
	}
		break;
	case Screen::Pause:
		if (cInput::GetKeyDown(27))
		{
			cGame::Get()->SetPaused(false);
			SetMenu(Screen::GameOverlay);
		}
		break;
	case Screen::Options:
		if (cInput::GetKeyDown(27))
		{
			SetMenu(Screen::Main);
			cSettings::Get()->Serialize();
		}
			
		break;
	case Screen::PauseOptions:
		if (cInput::GetKeyDown(27))
		{
			SetMenu(Screen::Pause);
			cSettings::Get()->Serialize();
		}
		break;
	case Screen::Instructions:
		if (cInput::GetAnyKeyDown())
			SetMenu(Screen::Main);
		break;
	default:
		break;
	}

	//Global elements
	timer += delta;
	if (timer > 1)
	{
		fps = frames;
		timer = 0;
		frames = 0;
	}
	((cGUILabel*)menus[(int)Screen::Global][0])->SetText("FPS: " + to_string(fps));

	int count = cGame::Get()->GetGameObjetsCount();
	((cGUILabel*)menus[(int)Screen::Global][1])->SetText("Objects: " + to_string(count));

	vector<cGUIElement *> menu = menus[(int)currentMenu];
	for (auto iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	frames++;
	
	if (currentMenu == Screen::Pause || currentMenu == Screen::PauseOptions) //yeah, dirty, I know
		for (auto iter = menus[(int)Screen::GameOverlay].begin(); iter != menus[(int)Screen::GameOverlay].end(); iter++)
			(*iter)->Render();
	vector<cGUIElement *> menu = menus[(int)currentMenu];
	for (auto iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Render();

	for (auto iter = menus[(int)Screen::Global].begin(); iter != menus[(int)Screen::Global].end(); iter++)
		(*iter)->Render();
}

void cGUI::ToggleBackground()
{
	cSettings::Get()->ToggleDrawBackground();
	UpdateBackgroundBtn();
}

void cGUI::ToggleVolume()
{
	float volume = cSettings::Get()->GetVolume() + 0.1f;
	if (volume >= 1.1f)
		volume = 0;
	cSettings::Get()->SetVolume(volume);
	UpdateVolumeBtn();
}

void cGUI::CleanUp()
{
	for (int i = 0; i < GUI_SCREENS; i++)
	{
		for (int j = 0; j < menus[i].size(); j++)
			delete menus[i][j];
		menus[i].clear();
	}
}

void cGUI::CleanUp(Screen menu)
{
	for (int i = 0; i < menus[(int)menu].size(); i++)
		delete menus[(int)menu][i];
	menus[(int)menu].clear();
}

void cGUI::UpdateVolumeBtn()
{
	float volume = cSettings::Get()->GetVolume();
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset + 1];
	btn->SetText("Volume: " + to_string((int)(volume * 100)) + "%");
}

void cGUI::UpdateBackgroundBtn()
{
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset];
	btn->SetText(cSettings::Get()->GetDrawBackground() ? "Background: On" : "Background: Off");
}

void cGUI::SetMenu(Screen menu)
{
	currentMenu = menu;
	switch (currentMenu)
	{
	case Screen::Death:
		CleanUp(Screen::Death);
		SetUpDeath();
		break;
	case Screen::PauseOptions:
	case Screen::Options:
		UpdateBackgroundBtn();
		UpdateVolumeBtn();
		break;
	case Screen::Highscores:
		CleanUp(Screen::Highscores);
		SetUpHightscores();
		break;
	default:
		break;
	}
}

void cGUI::SetUpMain()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	cGUIButton *btn = new cGUIButton(NULL, r, "Play Game", [this]() { cGame::Get()->StartLevel(0); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Instructions", [this]() { SetMenu(Screen::Instructions); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Highscores", [this]() { SetMenu(Screen::Highscores); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Options", [this]() { SetMenu(Screen::Options); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Quit", []() { exit(0); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);
}

void cGUI::SetUpInstructions()
{
	cGUIElement *elem = new cGUIElement(tutorialTexture, { 0, 0, windowSize.x, windowSize.y });
	menus[(int)Screen::Instructions].push_back(elem);
}

void cGUI::SetUpHightscores()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	int *scores = cSettings::Get()->GetScores();
	for (int i = 0; i < 10 && scores[i] != 0; i++)
	{
		r.top += emptySpace + 5; r.bottom += emptySpace + 5;
		cGUILabel *lbl = new cGUILabel(NULL, r, to_string(i + 1) + ". " + to_string(scores[i]));
		menus[(int)Screen::Highscores].push_back(lbl);
	}

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	cGUIButton *btn = new cGUIButton(NULL, r, "Back", [this](){ SetMenu(Screen::Main); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Highscores].push_back(btn);
}

void cGUI::SetUpOptions()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	string state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	cGUIButton *btn = new cGUIButton(NULL, r, "Background: " + state, [this]() { ToggleBackground(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Volume: " + state, [this]() { ToggleVolume(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Clear Scores", []() { cSettings::Get()->ClearScores(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back", [this]() { SetMenu(Screen::Main); cSettings::Get()->Serialize(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Options].push_back(btn);
}

void cGUI::SetUpGameOverlay()
{
	RECTF r = { windowSize.x / 2 - 100, windowSize.y - 40, windowSize.x / 2 + 100, windowSize.y };
	cGUIProgressBar *bar = new cGUIProgressBar(NULL, r, "", glm::vec4(1, 0, 0, 1));
	menus[(int)Screen::GameOverlay].push_back(bar);

	r.bottom -= 40; r.top -= 40;
	cGUILabel *lbl = new cGUILabel(NULL, r, "");
	menus[(int)Screen::GameOverlay].push_back(lbl);
}

void cGUI::SetUpPause()
{
	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::Pause].push_back(elem);

	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	cGUIButton *btn = new cGUIButton(NULL, r, "Resume", [this]() { cGame::Get()->SetPaused(false); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Pause].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Options", [this]() { SetMenu(Screen::PauseOptions); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Pause].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Main Menu", [this]() { cGame::Get()->Clear(); SetMenu(Screen::Main); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Pause].push_back(btn);
}

void cGUI::SetUpPauseOptions()
{
	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::PauseOptions].push_back(elem);
	
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	string state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	cGUIButton *btn = new cGUIButton(NULL, r, "Background: " + state, [this]() { ToggleBackground(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Volume: " + state, [this]() { ToggleVolume(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Clear Scores", []() { cSettings::Get()->ClearScores(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back", [this]() { SetMenu(Screen::Pause); cSettings::Get()->Serialize(); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::PauseOptions].push_back(btn);
}

void cGUI::SetUpDeath()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	cGUILabel *lbl = new cGUILabel(NULL, r, "The final score is: " + to_string(finalScore));
	menus[(int)Screen::Death].push_back(lbl);

	int *scores = cSettings::Get()->GetScores();
	bool wasFound = false;
	for (int i = 0; i < 10 && scores[i] != 0; i++)
	{
		r.top += emptySpace + 5; r.bottom += emptySpace + 5;
		lbl = new cGUILabel(NULL, r, to_string(i + 1) + ". " + to_string(scores[i]));
		if (!wasFound && scores[i] == finalScore)
		{
			lbl->SetColor(glm::vec3(1, 0, 0));
			wasFound = true;
		}
		menus[(int)Screen::Death].push_back(lbl);
	}

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	cGUIButton *btn = new cGUIButton(NULL, r, "Try Again", [this]() { cGame::Get()->Clear(); cGame::Get()->StartLevel(0); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Death].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back To Main", [this]() { SetMenu(Screen::Main); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Death].push_back(btn);
}

void cGUI::SetUpGlobalDataOverlay()
{
	cGUILabel *lbl = new cGUILabel(NULL, { windowSize.x - 80, 0, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 30, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
}