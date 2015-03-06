#include "cGUIElement.h"

void cGUIElement::Update()
{

}

void cGUIElement::Render()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, text->getTexture());

	glBegin(GL_QUADS);
		glColor3f(255.f, 255.f, 255.f);
		glTexCoord2i(0, 0);
		glVertex2i(rect.left, rect.top);
		glTexCoord2i(1, 0);
		glVertex2i(rect.right, rect.top);
		glTexCoord2i(1, 1);
		glVertex2i(rect.right, rect.bottom);
		glTexCoord2i(0, 1);
		glVertex2i(rect.left, rect.bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}