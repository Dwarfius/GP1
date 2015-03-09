#include "cBullet.h"
#include "cShip.h"

cBullet::cBullet(Owner pOwner, int pDamage) : 
	cGameObject(pOwner)
{
	damage = pDamage; 
	velocity = glm::vec2(500, 0);
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
	if (cShip *ship = dynamic_cast<cShip*>(col))
		ship->ApplyDamage(damage);
}