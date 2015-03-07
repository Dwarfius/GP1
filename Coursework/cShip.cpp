#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

void cShip::Update(float delta)
{
	velocity -= glm::sign(velocity) * 10 * delta;
	cGameObject::Update(delta);
}

void cShip::CollidedWith(cGameObject *col)
{

}

void cShip::Shoot(cGameObject *target)
{
	reloadTimer = timeToReload;
	cSprite *bulletSprite = new cSprite();
	bulletSprite->setTexture(missileText);
	bulletSprite->setSpriteScale(glm::vec2(0.5f, 0.5f));
	//cBullet *bullet = new cBullet(10);
	cMissile *bullet = new cMissile(target, 10);
	bullet->SetSprite(bulletSprite);
	bullet->SetRotation(sprite->getSpriteRotation());
	bullet->SetPosition(GetPosition() + forward * 40.f);
	bullet->SetVelocity(100);
	cGame::Get()->AddGameObject(bullet);
}