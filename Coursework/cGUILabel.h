#pragma once

#include "cGUIElement.h"

class cGUILabel :
	public cGUIElement
{
protected:
	string text;
	glm::vec2 textSize;
	glm::vec3 color = glm::vec3(1, 1, 1);
	void UpdateTextSize();

public:
	cGUILabel(cTexture *pTexture, RECTF pRect, string pText);
	~cGUILabel() {}

	void SetColor(glm::vec3 pColor) { color = pColor; }
	void SetText(string pText) { if (text != pText) { text = pText; UpdateTextSize(); }  }

	void Render() { Render(true); }
	void Render(bool useElemRender = true);
};

