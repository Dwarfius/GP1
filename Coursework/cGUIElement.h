#pragma once

#include "cTexture.h"

class cGUIElement
{
protected:
	cTexture *texture;
	RECTF rect;
	glm::vec2 center;
	glm::vec4 bkgdColor = glm::vec4(0, 0, 0, 0);

public:
	cGUIElement(cTexture *pTexture, RECTF pRect);
	~cGUIElement() {}

	virtual void SetBackgroundColor(glm::vec4 pBkgdColor) { bkgdColor = pBkgdColor; }

	virtual void Update();
	virtual void Render();
};

