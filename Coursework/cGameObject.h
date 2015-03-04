#pragma once

#include "cSprite.h"
#include "glm\glm.hpp"

class cGameObject
{
protected:
	cSprite *sprite;
	float velocity;
	float maxSpeed;
	float rotSpeed;
	float targetRot;
	glm::vec2 forward;
	bool destroy;

public:
	cGameObject();
	~cGameObject();

	glm::vec2 GetPosition() { return sprite->getSpritePos(); }
	void SetPosition(glm::vec2 pPos) { sprite->setSpritePos(pPos); }

	float GetVelocity() { return velocity; }
	void SetVelocity(float pVelocity) { velocity = glm::clamp(pVelocity, -maxSpeed, maxSpeed); }
	void AddVelocity(float delta) { velocity = glm::clamp(velocity + delta, -maxSpeed, maxSpeed); }

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