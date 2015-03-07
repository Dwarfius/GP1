#pragma once

#include "cGUIElement.h"

class cGUILabel :
	public cGUIElement
{
protected:
	string text;
	glm::vec2 textSize;
	glm::vec3 color = glm::vec3(255, 255, 255);

public:
	cGUILabel(cTexture *pTexture, RECTF pRect, string pText);
	~cGUILabel() {}

	void SetColor(glm::vec3 pColor) { color = pColor; }

	void Render(bool useElemRender = true);
};

