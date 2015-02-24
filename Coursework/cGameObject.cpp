#include "cGameObject.h"

cGameObject::cGameObject()
{
	maxSpeed = 100;
	rotSpeed = 50;
	velocity = 0;
	targetRot = 0;
	forward = glm::vec2(0, 1);
}

cGameObject::~cGameObject()
{
	if (sprite)
		delete sprite;
}

void cGameObject::Update(float delta)
{
	float angleDelta = targetRot - sprite->getSpriteRotation();
	if (glm::abs(angleDelta) > 180)
		angleDelta = (360 - glm::abs(angleDelta)) * -glm::sign(angleDelta);
	angleDelta = glm::clamp(angleDelta, -rotSpeed, rotSpeed);
	sprite->setSpriteRotation(sprite->getSpriteRotation() + angleDelta * delta);

	if (glm::abs(velocity) <= 1)
		velocity = 0;
	else
	{
		glm::vec2 pos = sprite->getSpritePos();
		pos += forward * velocity * delta;
		sprite->setSpritePos(pos);
		velocity -= glm::sign(velocity) * 10 * delta;
	}

	sprite->updateMatrix();
	sprite->updateBoundingRect();
	UpdateForward();
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

void cGameObject::UpdateForward()
{
	forward = glm::rotate(glm::vec2(0, -1), sprite->getSpriteRotation());
}