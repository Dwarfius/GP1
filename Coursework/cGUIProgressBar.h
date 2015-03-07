#pragma once

#include "cGUILabel.h"

class cGUIProgressBar :
	public cGUILabel
{
private:
	float percent;
	glm::vec3 fillColor;

public:
	cGUIProgressBar(cTexture *pTexture, RECTF pRect, string pText, glm::vec3 pFillColor);
	~cGUIProgressBar() {}

	void SetPercentage(float pPercent) { percent = pPercent; }
	void AddPercentage(float delta) { percent = glm::clamp(percent + delta, 0.f, 1.f); }
	float GetPercentage() { return percent; }

	void Render();
};

