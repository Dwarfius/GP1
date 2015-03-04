#pragma once

#include "cSprite.h"
#include "glm\glm.hpp"

class cGameObject
{
protected:
	cSprite *sprite;
	float velocity = 0;
	float maxVel = 100;
	float rotSpeed = 50;
	float targetRot = 0;
	glm::vec2 forward = glm::vec2(0, 1);
	bool destroy;

public:
	cGameObject() {} 
	~cGameObject() { if (sprite) delete sprite; }

	glm::vec2 GetPosition() { return sprite->getSpritePos(); }
	void SetPosition(glm::vec2 pPos) { sprite->setSpritePos(pPos); }

	float GetVelocity() { return velocity; }
	void SetVelocity(float pVelocity) { velocity = glm::clamp(pVelocity, -maxVel, maxVel); }
	void AddVelocity(float delta) { velocity = glm::clamp(velocity + delta, -maxVel, maxVel); }

	RECTF GetRect() { return sprite->getSpriteBoundingRect(); }

	char *GetData() { return sprite->GetData(); }

	glm::mat4x4 GetTransform() { return sprite->GetTransform(); }

	glm::vec2 GetSize() { return sprite->GetSize(); }

	bool IsDead() { return destroy; }
	void SetDestroy(bool flag) { destroy = flag; }

	void SetSprite(cSprite *s) { sprite = s; }
	virtual void Update(float delta);
	void Render();
	void LookAt(glm::vec2 target);
	void UpdateForward();
	virtual void CollidedWith(cGameObject *col);
};