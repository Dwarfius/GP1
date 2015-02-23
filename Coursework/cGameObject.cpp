#include "cGameObject.h"


cGameObject::cGameObject()
{
	maxSpeed = 10;
	rotSpeed = 20;
	velocity = glm::vec2(0);
	targetRot = 0;
	forward = glm::vec2(0, 1);
}

cGameObject::~cGameObject()
{
}

void cGameObject::Update(float delta)
{
	float angleDelta = targetRot - sprite->getSpriteRotation();
	if (glm::abs(angleDelta) > 180)
		angleDelta = (360 - glm::abs(angleDelta)) * -glm::sign(angleDelta);
	angleDelta = glm::clamp(angleDelta, -rotSpeed, rotSpeed);
	sprite->setSpriteRotation(sprite->getSpriteRotation() + angleDelta * delta);
}

void cGameObject::Render()
{
	sprite->render();
}

void cGameObject::LookAt(glm::vec2 target)
{
	glm::vec2 n = target - sprite->getSpritePos();
	float angle = glm::degrees(glm::atan(n.y, n.x));
	targetRot = angle + 90;
}