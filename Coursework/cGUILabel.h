#pragma once

#include "cGUIElement.h"

class cGUILabel :
	public cGUIElement
{
private:
	string text;
	glm::vec2 textSize;
	glm::vec3 color = glm::vec3(255, 255, 255);

public:
	cGUILabel(cTexture *pTexture, RECT pRect, string pText);
	~cGUILabel() {}

	void SetColor(glm::vec3 pColor) { color = pColor; }

	void Update();
	void Render();
};

