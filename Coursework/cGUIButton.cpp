#include "cGUIButton.h"
#include "cInput.h"

cGUIButton::cGUIButton(cTexture *pTexture, RECTF pRect, string pText, std::function<void()> pOnClick) :
	cGUILabel(pTexture, pRect, pText)
{
	onClick = pOnClick;
	savedColor = bkgdColor;
}

void cGUIButton::Update()
{
	glm::vec2 mPos = cInput::GetMousePos();
	bool inRect = RECTF::InRect(rect, mPos);
	if (inRect || active)
	{
		bkgdColor = highlightColor;

		if ((inRect && cInput::GetButtonDown(0)) || (active && cInput::GetControllerKeyDown(GamepadKeys::A)))
			onClick();
	}
	else
		bkgdColor = savedColor;
}

void cGUIButton::Render()
{
	cGUILabel::Render(true);
}