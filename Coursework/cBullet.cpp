#include "cBullet.h"
#include "cShip.h"

void cBullet::Update(float delta)
{
	currentLife += delta;
	if (currentLife > maxLife)
		destroy = true;

	cGameObject::Update(delta);
}

void cBullet::CollidedWith(cGameObject *col)
{
	destroy = true;
	if (cShip *ship = (cShip *)col)
		ship->ApplyDamage(damage);
}