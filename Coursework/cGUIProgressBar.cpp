#include "cGUIProgressBar.h"

#pragma warning(disable: 4244)

cGUIProgressBar::cGUIProgressBar(cTexture *pTexture, RECTF pRect, string pText, glm::vec3 pFillColor) :
	cGUILabel(pTexture, pRect, pText)
{
	fillColor = pFillColor;
}

void cGUIProgressBar::Render()
{
	cGUIElement::Render();

	glEnable(GL_TEXTURE_2D);

	float width = (rect.right - rect.left) * percent;
	glBegin(GL_QUADS);
		glColor3f(fillColor.x, fillColor.y, fillColor.z);
		glVertex2i(rect.left, rect.top);
		glVertex2i(rect.left + width, rect.top);
		glVertex2i(rect.left + width, rect.bottom);
		glVertex2i(rect.left, rect.bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	cGUILabel::Render(false);
}

