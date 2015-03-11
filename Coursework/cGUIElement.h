#pragma once

#include "cTexture.h"

class cGUIElement
{
protected:
	cTexture *texture;
	RECTF rect;
	glm::vec2 center;
	glm::vec4 bkgdColor = glm::vec4(1, 1, 1, 1);

public:
	cGUIElement(cTexture *pTexture, RECTF pRect);
	~cGUIElement() {}

	virtual void SetBackgroundColor(glm::vec4 pBkgdColor) { bkgdColor = pBkgdColor; }

	virtual void Update();
	virtual void Render();

	void SetRect(RECTF pRect) { rect = pRect; center = glm::vec2(rect.left + rect.right, rect.bottom + rect.top) * 0.5f; }
};

