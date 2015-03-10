#include "cGUI.h"
#include "GameConstants.h"
#include "cGUILabel.h"
#include "cGUIButton.h"
#include "cGUIProgressBar.h"
#include "cGame.h"
#include "cInput.h"

#pragma warning(disable: 4018 4244)

cGUI::cGUI(glm::vec2 pWindowSize)
{
	windowSize = pWindowSize;
	SetUp();
}

void cGUI::UpdateSize(glm::vec2 newSize)
{
	windowSize = newSize;
	SetUp();
}

void cGUI::SetUp()
{
	for (int i = 0; i < 5; i++)
		menus[i].clear();

	float btnWidth = 100;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	//Main Menu
	cGUIButton *btn = new cGUIButton(NULL, r, "Play Game", [this]() { cGame::Get()->StartLevel(0); currentMenu++; });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[0].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Quit", []() { exit(0); });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[0].push_back(btn);

	//Game Overlay
	r = { windowSize.x / 2 - 100, windowSize.y - 40, windowSize.x / 2 + 100, windowSize.y };
	cGUIProgressBar *bar = new cGUIProgressBar(NULL, r, "", glm::vec4(1, 0, 0, 1));
	menus[1].push_back(bar);

	r.bottom -= 40; r.top -= 40;
	cGUILabel *lbl = new cGUILabel(NULL, r, "");
	menus[1].push_back(lbl);

	//Pause Menu
	menus[2].push_back(bar); //I want health bar to be visible in the pause screen
	menus[2].push_back(lbl);

	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[2].push_back(elem);

	x = (windowSize.x - btnWidth) / 2;
	y = windowSize.y / 4;
	r = { x, y, x + btnWidth, y + btnHeight };
	btn = new cGUIButton(NULL, r, "Resume", [this]() { cGame::Get()->SetPaused(false); currentMenu = 1; });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[2].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Main Menu", [this]() { cGame::Get()->Clear(); currentMenu = 0; });
	btn->SetBackgroundColor(glm::vec4(0, 0, 1, 1));
	btn->SetHighlightColor(glm::vec4(0.3f, 0.3f, 1, 1));
	menus[2].push_back(btn);

	//global fps counter
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 0, windowSize.x, 20 }, "");
	menus[3].push_back(lbl);
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 30, windowSize.x, 20 }, "");
	menus[3].push_back(lbl);
}

void cGUI::Update(float delta)
{
	//per menu elements
	switch (currentMenu)
	{
	case 1:
	{
		cPlayer *player = cGame::Get()->GetPlayer();
		float health = player->GetHealth();
		float max = player->GetMaxHealth();
		cGUIProgressBar *bar = (cGUIProgressBar*)menus[1][0];
		bar->SetPercentage(health / max);
		bar->SetText(to_string((int)health) + "/" + to_string((int)max));

		cGUILabel *lbl = (cGUILabel*)menus[1][1];
		lbl->SetText("Score: " + to_string(cGame::Get()->GetScore()));

		if (cInput::GetKeyDown(27))
		{
			currentMenu = 2;
			cGame::Get()->SetPaused(true);
		}
			
	}
		break;
	case 2:
		if (cInput::GetKeyDown(27))
		{
			cGame::Get()->SetPaused(false);
			currentMenu = 1;
		}
		break;
	default:
		break;
	}

	//Global elements
	timer += delta;
	if (timer > 1)
	{
		fps = frames / timer;
		timer = 0;
		frames = 0;
	}
	((cGUILabel*)menus[3][0])->SetText("FPS: " + to_string(fps));

	int count = cGame::Get()->GetGameObjetsCount();
	((cGUILabel*)menus[3][1])->SetText("Objects: " + to_string(count));

	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	frames++;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowSize.x, windowSize.y, 0, -1, 1);
	
	vector<cGUIElement *> menu = menus[currentMenu];
	for (auto iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Render();

	for (auto iter = menus[3].begin(); iter != menus[3].end(); iter++)
		(*iter)->Render();
}