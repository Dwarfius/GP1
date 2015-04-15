#include "cGUIButton.h"
#include "cInput.h"

cGUIButton::cGUIButton(cTexture *pTexture, RECTF pRect, string pText) :
	cGUILabel(pTexture, pRect, pText)
{
	savedColor = bkgdColor; //caching the normal color
}

void cGUIButton::Update()
{
	glm::vec2 mPos = cInput::GetMousePos();
	bool inRect = RECTF::InRect(rect, mPos);
	if (inRect || active)
	{
		bkgdColor = highlightColor; //change color if selected

		if ((inRect && cInput::GetButtonDown(0)) ||
			(active && (cInput::GetControllerKeyDown(GamepadKeys::A) || cInput::GetKeyDown(32))))
			onClick(); //call the callback
	}
	else
		bkgdColor = savedColor;
}

void cGUIButton::Render()
{
	cGUILabel::Render(true);
}