#include "cGUILabel.h"

#pragma warning(disable : 4018)

cGUILabel::cGUILabel(cTexture *pTexture, RECTF pRect, string pText) :
	cGUIElement(pTexture, pRect)
{
	text = pText;
	UpdateTextSize();
}

void cGUILabel::UpdateTextSize()
{
	int width = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, reinterpret_cast<const unsigned char *>(text.c_str()));
	int height = 12;
	textSize = glm::vec2(width, height);
}

void cGUILabel::Render(bool useElemRender)
{
	if (useElemRender)
		cGUIElement::Render();

	glColor3f(color.x, color.y, color.z);
	float x = center.x - textSize.x / 2;
	for (int i = 0; i < text.size(); i++)
	{
		int step = glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, text[i]);
		glRasterPos2f(x, center.y - textSize.y / 2 + 10);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]); //http://openglut.sourceforge.net/group__bitmapfont.html#ga2
		x += step;
	}
}