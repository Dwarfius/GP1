#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"

class cGUI
{
private:
	int currentMenu = 0;
	vector<cGUIElement *> menus[5];
	bool active = true;

public:
	cGUI();
	~cGUI() {}

	void Update();
	void Render(glm::vec2 offset);
};

