#pragma once
#include "cTexture.h"
class cGUIElement
{
private:
	cTexture *text;
	RECT rect;

public:
	cGUIElement(cTexture *pText, RECT pRect) { text = pText; rect = pRect; }
	~cGUIElement() {}
	virtual void Update();
	virtual void Render();
};

