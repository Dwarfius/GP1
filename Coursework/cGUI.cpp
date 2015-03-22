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
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	//Main Menu
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
	btn = new cGUIButton(NULL, r, "Options", [this]() { SetMenu(Screen::Options); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Quit", []() { exit(0); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Main].push_back(btn);

	//Instructions Screen
	cGUIElement *elem = new cGUIElement(tutorialTexture, { 0, 0, windowSize.x, windowSize.y });
	menus[(int)Screen::Instructions].push_back(elem);

	//Options Menu
	r = { x, y, x + btnWidth, y + btnHeight };
	string state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	btn = new cGUIButton(NULL, r, "Background: " + state, [this]() { ToggleBackground(); });
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
	btn = new cGUIButton(NULL, r, "Back", [this]() { SetMenu(Screen::Main); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Options].push_back(btn);

	//Game Overlay
	r = { windowSize.x / 2 - 100, windowSize.y - 40, windowSize.x / 2 + 100, windowSize.y };
	cGUIProgressBar *bar = new cGUIProgressBar(NULL, r, "", glm::vec4(1, 0, 0, 1));
	menus[(int)Screen::GameOverlay].push_back(bar);

	r.bottom -= 40; r.top -= 40;
	cGUILabel *lbl = new cGUILabel(NULL, r, "");
	menus[(int)Screen::GameOverlay].push_back(lbl);

	//Pause Menu
	elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::Pause].push_back(elem);

	x = (windowSize.x - btnWidth) / 2;
	y = windowSize.y / 4;
	r = { x, y, x + btnWidth, y + btnHeight };
	btn = new cGUIButton(NULL, r, "Resume", [this]() { cGame::Get()->SetPaused(false); SetMenu(Screen::GameOverlay); });
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

	//Pause Options Menu
	elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::PauseOptions].push_back(elem);

	r = { x, y, x + btnWidth, y + btnHeight };
	state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	btn = new cGUIButton(NULL, r, "Background: " + state, [this]() { ToggleBackground(); });
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
	btn = new cGUIButton(NULL, r, "Back", [this]() { SetMenu(Screen::Pause); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::PauseOptions].push_back(btn);

	//Death Menu
	x = (windowSize.x - btnWidth) / 2;
	y = windowSize.y / 4;
	r = { x, y, x + btnWidth, y + btnHeight };
	lbl = new cGUILabel(NULL, r, "");
	menus[(int)Screen::Death].push_back(lbl);
	
	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Try Again", [this]() { cGame::Get()->Clear(); cGame::Get()->StartLevel(0); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Death].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back To Main", [this]() { SetMenu(Screen::Main); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[(int)Screen::Death].push_back(btn);

	//Global Data Overlay
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 0, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 30, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
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
			SetMenu(Screen::Main);
		break;
	case Screen::PauseOptions:
		if (cInput::GetKeyDown(27))
			SetMenu(Screen::Pause);
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

void cGUI::SetFinalScore(int score)
{
	cGUILabel *lbl = (cGUILabel*)menus[(int)Screen::Death][0];
	lbl->SetText("The final score is: " + to_string(score));
}

void cGUI::ToggleBackground()
{
	cSettings::Get()->ToggleDrawBackground();
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset];
	btn->SetText(cSettings::Get()->GetDrawBackground() ? "Background: On" : "Background: Off");
}

void cGUI::ToggleVolume()
{
	float volume = cSettings::Get()->GetVolume() + 0.1f;
	if (volume >= 1.1f)
		volume = 0;
	cSettings::Get()->SetVolume(volume);
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset + 1];
	btn->SetText("Volume: " + to_string((int)(volume * 100)) + "%");
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