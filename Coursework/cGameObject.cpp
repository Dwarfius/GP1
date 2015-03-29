#include "cGameObject.h"

cGameObject::cGameObject(cTexture *pText, Owner pOwner)
{
	owner = pOwner;
	sprite = new cSprite();
	sprite->setTexture(pText);
}

void cGameObject::Update(float delta)
{
	if (destroy)
		return;

	float angleDelta = WrapAngle(targetRot - sprite->getSpriteRotation());
	angleDelta = glm::clamp(angleDelta, -rotSpeed * delta, rotSpeed * delta);
	sprite->setSpriteRotation(sprite->getSpriteRotation() + angleDelta);

	float length2 = glm::length2(velocity);
	if (length2 <= 1)
		velocity = glm::vec2(0, 0);
	else
	{
		if (length2 > maxVel * maxVel)
			velocity = glm::normalize(velocity) * maxVel;

		glm::vec2 pos = sprite->getSpritePos();
		pos += velocity * delta;
		sprite->setSpritePos(pos);
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

void cGameObject::CollidedWith(cGameObject *col)
{
	cout << GetName() << " collided with " << col->GetName() << endl;
}

//http://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
float cGameObject::WrapAngle(float angle)
{
	float ret = glm::mod(angle + 180, 360.f);
	if (ret < 0)
		ret += 360;
	ret -= 180;
	return ret;
}