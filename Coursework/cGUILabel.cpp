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
	halfWidths.clear();
	halfHeight = 0; //starting from 10 so that it's line1 => emptySpace + line2 => emptySpace + line3 =>...
	for (int i = 0; i < text.size(); i++)
	{
		bool newLine = text[i] == '\n'; //parsing up the lines
		bool end = i == text.size() - 1;

		if (newLine)
			halfHeight += 14; //10 for font height, 4 for whitespace
		else if (end)
			halfHeight += 10;

		if (newLine || end)
		{
			string t = text.substr(0, i+1); //including \n
			lines.push_back(t);
			int width = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, reinterpret_cast<const unsigned char *>(t.c_str()));
			halfWidths.push_back(width / 2);
			text = text.substr(i + 1);
			i = -1;
		}
	}
	halfHeight /= 2;
}

void cGUILabel::Render(bool useElemRender)
{
	if (useElemRender) //making sure the text is rendered over the background
		cGUIElement::Render();

	glColor3f(color.x, color.y, color.z); //setting the color
	for (int i = 0; i < lines.size(); i++)
	{
		float x, y;
		if (centered)
		{
			x = center.x - halfWidths[i];
			y = center.y - halfHeight + 14 * i + 8; //I need to offset it by 8 to center it	
		}							   //also, multyplying by 14 so that there's 4 pixels of spacing
		else
		{
			x = rect.left;
			y = rect.top + 14 * i + 8;
		}
		glRasterPos2f(x, y); //update position for each new line

		string line = lines[i];
		for (int c = 0; c < line.size(); c++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line[c]);
	}
}