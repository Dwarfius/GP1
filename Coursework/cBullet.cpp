#include "cBullet.h"
#include "cShip.h"

cBullet::cBullet(int pDamage)
{
	damage = pDamage; 
	velocity = glm::vec2(100, 0);
}

void cBullet::Update(float delta)
{
	if (destroy)
		return;

	currentLife -= delta;
	if (currentLife < 0)
		destroy = true;

	cGameObject::Update(delta);
}

void cBullet::CollidedWith(cGameObject *col)
{
	destroy = true;
	if (cShip *ship = (cShip *)col)
		ship->ApplyDamage(damage);
}