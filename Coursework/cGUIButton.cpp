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
	if (RECTF::InRect(rect, mPos))
	{
		bkgdColor = highlightColor;

		if (cInput::GetButtonDown(0))
			onClick();
	}
	else
		bkgdColor = savedColor;
}

void cGUIButton::Render()
{
	cGUILabel::Render(true);
}