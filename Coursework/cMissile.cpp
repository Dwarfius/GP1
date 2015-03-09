#include "cMissile.h"
#include "cShip.h"

cMissile::cMissile(Owner pOwner, cGameObject *pTarget, int pDamage) :
	cGameObject(pOwner)
{
	target = pTarget; 
	damage = pDamage;
	velocity = glm::vec2(300, 0);
	rotSpeed = 100;
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
	if (cShip *ship = dynamic_cast<cShip*>(col))
		ship->ApplyDamage(damage);
}