#include "cMissile.h"
#include "cShip.h"

cMissile::cMissile(cGameObject *pTarget, int pDmg)
{
	target = pTarget; 
	damage = pDmg;
	SetVelocity(100);
}

void cMissile::Update(float delta)
{
	currentLife -= delta;
	if (currentLife < 0)
		destroy = true;

	if (target)
	{
		if (target->IsDead())
			target = NULL;
		else
			LookAt(target->GetPosition());
	}

	cGameObject::Update(delta);
}

void cMissile::CollidedWith(cGameObject *col)
{
	destroy = true;
	if (cShip *ship = (cShip*)col)
		ship->ApplyDamage(damage);
}