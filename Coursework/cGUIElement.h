#pragma once

#include "cTexture.h"

class cGUIElement
{
protected:
	cTexture *texture;
	RECTF rect;
	glm::vec2 center;
	glm::vec3 bkgdColor = glm::vec3(0, 0, 0);

public:
	cGUIElement(cTexture *pTexture, RECTF pRect);
	~cGUIElement() {}

	void SetBackgroundColor(glm::vec3 pBkgdColor) { bkgdColor = pBkgdColor; }

	virtual void Update();
	virtual void Render();
};

