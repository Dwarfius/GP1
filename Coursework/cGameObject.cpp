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

	//limited-delta rotation
	float angleDelta = WrapAngle(targetRot - sprite->getSpriteRotation());
	angleDelta = glm::clamp(angleDelta, -rotSpeed * delta, rotSpeed * delta);
	sprite->setSpriteRotation(sprite->getSpriteRotation() + angleDelta);

	//move along velocity
	glm::vec2 pos = sprite->getSpritePos();
	pos += velocity * delta;
	sprite->setSpritePos(pos);

	//since we moved, update transform and bounding rect
	sprite->updateMatrix();
	sprite->updateBoundingRect();
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

//collision plug
void cGameObject::CollidedWith(cGameObject *col)
{
}

//thanks
//http://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
float cGameObject::WrapAngle(float angle)
{
	float ret = glm::mod(angle + 180, 360.f);
	if (ret < 0)
		ret += 360;
	ret -= 180;
	return ret;
}