#pragma once

#include "GameConstants.h"

class cSprite;

class cGameObject
{
private:
	cSprite *sprite;
	glm::vec2 velocity;
	float maxSpeed;
	float rotSpeed;
	float targetRot;
	glm::vec2 forward;

public:
	cGameObject();
	~cGameObject();

	void SetSprite(cSprite *s) { sprite = s; }
	void Update(float delta);
	void Render();
	void LookAt(glm::vec2 target);
};