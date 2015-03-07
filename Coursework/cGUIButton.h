#pragma once

#include "cGUILabel.h"
#include <functional>

class cGUIButton :
	public cGUILabel
{
private:
	glm::vec3 highlightColor;
	glm::vec3 savedColor;
	std::function<void()> onClick;

public:
	cGUIButton(cTexture *pTexture, RECTF pRect, string pText, std::function<void()> pOnClick);
	~cGUIButton() {}

	void SetBackgroundColor(glm::vec3 pBkgdColor) { savedColor = pBkgdColor; }
	void SetHighlightColor(glm::vec3 pHighlightColor) { highlightColor = pHighlightColor; }

	void Update();
	void Render();
};

