#include "cBullet.h"
#include "cShip.h"
#include "cGame.h"

cBullet::cBullet(cTexture *pText, Owner pOwner, int pDamage) : 
	cGameObject(pText, pOwner)
{
	damage = pDamage; 
	cGame::Get()->PlaySnd("bullet", false); //play on creation
}

void cBullet::Update(float delta)
{
	if (destroy)
		return;

	currentLife -= delta; //has a life limit
	if (currentLife < 0)
		destroy = true;

	//just move forward every update
	glm::vec2 pos = sprite->getSpritePos();
	pos += velocity * delta;
	sprite->setSpritePos(pos);

	sprite->updateMatrix();
	sprite->updateBoundingRect();
}

void cBullet::CollidedWith(cGameObject *col)
{
	destroy = true;
	if (cShip *ship = dynamic_cast<cShip*>(col)) //only ships receive damage
		ship->ApplyDamage(damage);
}