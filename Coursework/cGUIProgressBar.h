#pragma once

#include "cGUILabel.h"

class cGUIProgressBar :
	public cGUILabel
{
private:
	float percent;
	glm::vec4 fillColor;

public:
	cGUIProgressBar(cTexture *pTexture, RECTF pRect, string pText, glm::vec4 pFillColor);
	~cGUIProgressBar() {}

	void SetPercentage(float pPercent) { percent = glm::clamp(pPercent, 0.f, 1.f); }
	void AddPercentage(float delta) { percent = glm::clamp(percent + delta, 0.f, 1.f); }
	float GetPercentage() { return percent; }

	void Render();
};

