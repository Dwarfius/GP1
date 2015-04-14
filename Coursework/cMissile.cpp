#include "cMissile.h"
#include "cShip.h"

cMissile::cMissile(cTexture *pText, Owner pOwner, cGameObject *pTarget, int pDamage) :
	cGameObject(pText, pOwner)
{
	target = pTarget; 
	damage = pDamage;
	rotSpeed = 80;
}

void cMissile::Update(float delta)
{
	if (destroy)
		return;

	velocity = 300.f * GetForward();

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
	if (cShip *ship = dynamic_cast<cShip*>(col))
		ship->ApplyDamage(damage);
}