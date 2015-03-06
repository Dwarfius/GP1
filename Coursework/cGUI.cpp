#include "cGUI.h"
#include "GameConstants.h"

cGUI::cGUI()
{
	//fill menus here
}

void cGUI::Update()
{
	if (!active)
		return;

	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin; iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	if (!active)
		return;

	glMatrixMode(GL_PROJECTION);
	glTranslatef(offest.x - WINDOW_WIDTH / 2, offest.y - WINDOW_HEIGHT / 2 + 8, 0); //have to offset it by 8 so that it's visible
	
	vector<cGUIElement *> menu = menus[currentMenu];
	for (vector<cGUIElement *>::iterator iter = menu.begin; iter != menu.end(); iter++)
		(*iter)->Render();
}