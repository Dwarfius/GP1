#pragma once

#include "cSprite.h"
#include "glm\glm.hpp"

enum class Owner { Player, Enemy, Neutral };

class cGameObject
{
protected:
	cSprite *sprite;
	glm::vec2 velocity = glm::vec2(0, 0);
	float maxVel = 10000000;
	float rotSpeed = 100;
	float targetRot = 0;
	glm::vec2 forward = glm::vec2(0, 1);
	bool destroy;
	Owner owner;
	bool dirty = true;

public:
	cGameObject(cTexture *pText, Owner pOwner);
	~cGameObject() { if (sprite) delete sprite; }

	glm::vec2 GetPosition() { return sprite->getSpritePos(); }
	void SetPosition(glm::vec2 pPos) { sprite->setSpritePos(pPos); }

	float GetRotation() { return sprite->getSpriteRotation(); }
	void SetRotation(float rot) { sprite->setSpriteRotation(rot); targetRot = rot; UpdateForward(); }

	glm::vec2 GetVelocity() { return velocity; }
	void SetVelocity(glm::vec2 pVelocity) { velocity = pVelocity; }
	void AddVelocity(glm::vec2 delta) { velocity += delta; }

	RECTF GetRect() { return sprite->getSpriteBoundingRect(); }

	char *GetData() { return sprite->GetData(); }

	glm::mat4x4 GetTransform() { return sprite->GetTransform(); }

	glm::vec2 GetSize() { return sprite->GetSize(); }

	glm::vec2 GetForward() { return forward; }
	glm::vec2 GetRight() { return glm::vec2(forward.y, -forward.x); }

	bool IsDead() { return destroy; }
	void SetDestroy(bool flag) { destroy = flag; }

	void SetSprite(cSprite *s) { sprite = s; }
	virtual void Update(float delta);
	void Render();
	void LookAt(glm::vec2 target);
	void UpdateForward();
	virtual void CollidedWith(cGameObject *col);
	virtual string GetName() { return "GameObject"; }
	Owner GetOwner() { return owner; }
	virtual void OnDestroy() {};
	void UpdateBoundingRect();
};