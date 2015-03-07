#include "cGUI.h"
#include "GameConstants.h"
#include "cGUILabel.h"
#include "cGUIButton.h"
#include "cGUIProgressBar.h"
#include "cGame.h"

#pragma warning(disable: 4244)

cGUI::cGUI(glm::vec2 pWindowSize)
{
	windowSize = pWindowSize;
	float btnWidth = 100;
	float btnHeight = 25;
	float emptySpace = 10;

	//MainMenu
	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 8;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	cGUIButton *btn = new cGUIButton(NULL, r, "Play Game", [this]() { cGame::Get()->StartLevel(0); currentMenu++; });
	btn->SetBackgroundColor(glm::vec3(0, 0, 1));
	btn->SetHighlightColor(glm::vec3(0.3f, 0.3f, 1));
	menus[0].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Quit", []() { exit(0); });
	btn->SetBackgroundColor(glm::vec3(0, 0, 1));
	btn->SetHighlightColor(glm::vec3(0.3f, 0.3f, 1));
	menus[0].push_back(btn);

	//GameOverlay
	r = { windowSize.x / 2 - 100, windowSize.y - 40, windowSize.x / 2 + 100, windowSize.y };
	cGUIProgressBar *bar = new cGUIProgressBar(NULL, r, "", glm::vec3(1, 0, 0));
	menus[1].push_back(bar);
}

void cGUI::UpdateSize(glm::vec2 newSize)
{

}

void cGUI::Update(float delta)
{
	if (!active)
		return;

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
	}
		break;
	default:
		break;
	}

	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	if (!active)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowSize.x, windowSize.y, 0, -1, 1);
	
	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Render();
}