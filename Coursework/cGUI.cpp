#include "cGUI.h"
#include "GameConstants.h"
#include "cGUILabel.h"
#include "cGUIButton.h"
#include "cGUIProgressBar.h"

cGUI::cGUI()
{
	RECTF r1 = { 0, 0, 100, 100 };
	RECTF r2 = { 100, 0, 200, 100 };
	cGUIButton *btn = new cGUIButton(NULL, r1, "Hello World", []() { cout << "test" << endl; });
	btn->SetHighlightColor(glm::vec3(255, 0, 0));
	menus[0].push_back(btn);
	menus[0].push_back(new cGUIProgressBar(NULL, r2, "Hello World", glm::vec3(255, 0, 0)));
}

void cGUI::Update(float delta)
{
	if (!active)
		return;

	cGUIProgressBar *bar = (cGUIProgressBar*)menus[0][1];
	bar->AddPercentage(delta / 20);

	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	if (!active)
		return;

	glMatrixMode(GL_PROJECTION);
	glTranslatef(offest.x - WINDOW_WIDTH / 2, offest.y - WINDOW_HEIGHT / 2, 0);
	
	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Render();
}