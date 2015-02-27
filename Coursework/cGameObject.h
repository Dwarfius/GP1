#pragma once

#include "cSprite.h"
#include "glm\glm.hpp"

class cGameObject
{
private:
	cSprite *sprite;
	float velocity;
	float maxSpeed;
	float rotSpeed;
	float targetRot;
	glm::vec2 forward;

public:
	cGameObject();
	~cGameObject();

	glm::vec2 GetPosition() { return sprite->getSpritePos(); }
	void SetPosition(glm::vec2 pPos) { sprite->setSpritePos(pPos); }

	float GetVelocity() { return velocity; }
	void SetVelocity(float pVelocity) { velocity = glm::clamp(pVelocity, -maxSpeed, maxSpeed); }
	void AddVelocity(float delta) { velocity = glm::clamp(velocity + delta, -maxSpeed, maxSpeed); }

	RECTF GetRect() { return sprite->getSpriteBoundingRect(); }

	void SetSprite(cSprite *s) { sprite = s; }
	void Update(float delta);
	void Render();
	void LookAt(glm::vec2 target);
	void UpdateForward();
};