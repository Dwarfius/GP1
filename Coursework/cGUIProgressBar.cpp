#include "cGUIProgressBar.h"

#pragma warning(disable: 4244)

cGUIProgressBar::cGUIProgressBar(cTexture *pTexture, RECTF pRect, string pText, glm::vec4 pFillColor) :
	cGUILabel(pTexture, pRect, pText)
{
	fillColor = pFillColor;
}

void cGUIProgressBar::Render()
{
	cGUIElement::Render(); //rendering background

	glEnable(GL_TEXTURE_2D);

	float width = (rect.right - rect.left) * percent; //calculating the fill amount
	glColor4f(fillColor.x, fillColor.y, fillColor.z, fillColor.w);
	glBegin(GL_QUADS);
		glVertex2i(rect.left, rect.top);
		glVertex2i(rect.left + width, rect.top); //fill it
		glVertex2i(rect.left + width, rect.bottom);
		glVertex2i(rect.left, rect.bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	cGUILabel::Render(false); //no need to render background, since already done
}

