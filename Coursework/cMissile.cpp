#include "cMissile.h"
#include "cShip.h"

cMissile::cMissile(cGameObject *pTarget, int pDamage)
{
	target = pTarget; 
	damage = pDamage;
	velocity = glm::vec2(100, 0);
}

void cMissile::Update(float delta)
{
	if (destroy)
		return;

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