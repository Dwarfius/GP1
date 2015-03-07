#include "cGUIElement.h"

cGUIElement::cGUIElement(cTexture *pTexture, RECTF pRect) 
{ 
	texture = pTexture; 
	rect = pRect; 
	center = glm::vec2(rect.left + rect.right, rect.top + rect.bottom) * 0.5f;
}

void cGUIElement::Update()
{

}

void cGUIElement::Render()
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, texture ? texture->getTexture() : 0);

	glBegin(GL_QUADS);
		glColor3f(bkgdColor.x, bkgdColor.y, bkgdColor.z);
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