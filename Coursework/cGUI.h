#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"

class cGUI
{
private:
	glm::vec2 windowSize;
	int currentMenu = 0;
	vector<cGUIElement *> menus[5];

	void SetUp();

public:
	cGUI(glm::vec2 pWindowSize);
	~cGUI() {}

	void UpdateSize(glm::vec2 newSize);

	void Update(float delta);
	void Render(glm::vec2 offset);
};

