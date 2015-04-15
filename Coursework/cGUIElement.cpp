#include "cGUIElement.h"

#pragma warning(disable: 4244)

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

	glColor4f(bkgdColor.x, bkgdColor.y, bkgdColor.z, bkgdColor.w); //setting the current background color
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(rect.left, rect.top); //since not using a transform, rendering in proper absolutes
		glTexCoord2i(1, 0);
		glVertex2i(rect.right, rect.top);
		glTexCoord2i(1, 1);
		glVertex2i(rect.right, rect.bottom);
		glTexCoord2i(0, 1);
		glVertex2i(rect.left, rect.bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}