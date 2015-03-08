#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

void cShip::Update(float delta)
{
	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

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
	bullet->SetVelocity(bullet->GetForward() * 100.f);
	cGame::Get()->AddGameObject(bullet);
}