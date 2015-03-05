#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"

void cShip::Update(float delta)
{
	velocity -= glm::sign(velocity) * 10 * delta;
	cGameObject::Update(delta);
}

void cShip::CollidedWith(cGameObject *col)
{

}

void cShip::Shoot()
{
	reloadTimer = timeToReload;
	cSprite *bulletSprite = new cSprite();
	bulletSprite->setTexture(missileText);
	bulletSprite->setSpriteScale(glm::vec2(0.25f, 0.25f));
	cBullet *bullet = new cBullet(10);
	bullet->SetSprite(bulletSprite);
	bullet->SetRotation(sprite->getSpriteRotation());
	bullet->SetPosition(GetPosition() + forward * 40.f);
	bullet->SetVelocity(100);
	cGame::Get()->AddGameObject(bullet);
}