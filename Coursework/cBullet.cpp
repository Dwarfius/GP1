#include "cBullet.h"
#include "cShip.h"

void cBullet::Update(float delta)
{
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
	cout << "Bullet collided with " << typeid(col).name() << endl;
}