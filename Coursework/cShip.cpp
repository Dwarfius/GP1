#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

void cShip::Update(float delta)
{
	if (destroy)
		return;

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	cGameObject::Update(delta);
}

void cShip::CollidedWith(cGameObject *col)
{

}

void cShip::Shoot(cGameObject *target)
{
	glm::vec2 spawnPos = GetPosition() + forward * 40.f;
	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
	{
		if ((*iter)->CanShoot())
			(*iter)->Shoot(spawnPos, GetRotation(), target);
	}
}