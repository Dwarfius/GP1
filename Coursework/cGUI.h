#pragma once

#include "glm\glm.hpp"
#include "cGUIElement.h"

//glRasterPos2f(0, 0);
//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 't'); http://openglut.sourceforge.net/group__bitmapfont.html#ga2

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

