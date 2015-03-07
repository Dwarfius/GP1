#include "cGUIButton.h"
#include "cInput.h"

cGUIButton::cGUIButton(cTexture *pTexture, RECTF pRect, string pText, std::function<void()> pFunc) :
	cGUILabel(pTexture, pRect, pText)
{
	func = pFunc;
}

void cGUIButton::Update()
{
	glm::vec2 mPos = cInput::GetMousePos();
	if (RECTF::InRect(rect, mPos))
	{
		savedColor = bkgdColor;
		bkgdColor = highlightColor;

		if (cInput::GetButtonDown(0))
			func();
	}
	else
		bkgdColor = savedColor;
}

void cGUIButton::Render()
{
	cGUILabel::Render();
}