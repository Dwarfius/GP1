#include "cGUILabel.h"

#pragma warning(disable : 4018)

cGUILabel::cGUILabel(cTexture *pTexture, RECTF pRect, string pText, bool pCentered) :
	cGUIElement(pTexture, pRect)
{
	text = pText;
	bkgdColor = glm::vec4(0, 0, 0, 0);
	centered = pCentered;
	UpdateTextSize();
}

void cGUILabel::UpdateTextSize()
{ 
	lines.clear();
	widths.clear();
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n' || i == text.size() - 1)
		{
			string t = text.substr(0, i+1);
			lines.push_back(t);
			int width = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, reinterpret_cast<const unsigned char *>(t.c_str()));
			widths.push_back(width);
			text = text.substr(i + 1);
			i = -1;
		}
	}
}

void cGUILabel::Render(bool useElemRender)
{
	if (useElemRender)
		cGUIElement::Render();

	glColor3f(color.x, color.y, color.z);
	for (int i = 0; i < lines.size(); i++)
	{
		float x, y;
		if (centered)
		{
			x = center.x - widths[i] / 2;
			y = center.y - 10 * (lines.size()-1) + 14 * i + 5; //although it's helvetica12, the height is kinda 10 pixels, so I need to offset it by 5 to center		
		}							   //also, multyplying by 14 so that there's 4 pixels of spacing
		else
		{
			x = rect.left;
			y = rect.top + 14 * i + 5;
		}
		
		string line = lines[i];
		for (int c = 0; c < line.size(); c++)
		{
			glRasterPos2f(x, y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line[c]);

			int step = glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, line[c]); //http://openglut.sourceforge.net/group__bitmapfont.html#ga2
			x += step;
		}
	}
}