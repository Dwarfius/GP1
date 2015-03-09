#include "cWeapon.h"
#include "cBullet.h"
#include "cMissile.h"
#include "cGame.h"

cWeapon::cWeapon(cTexture *pProjTexture, float pReloadTimer, WeaponType pType, int pDamage)
{
	projTexture = pProjTexture;
	reloadTimer = pReloadTimer;
	type = pType;
	damage = pDamage;
}

void cWeapon::Update(float delta)
{
	reload -= delta;
}

void cWeapon::Shoot(Owner owner, glm::vec2 spawnPos, float angle, cGameObject *target)
{
	if (type == WeaponType::Missile && target == NULL)
		return;

	cSprite *sprite = new cSprite();
	sprite->setTexture(projTexture);
	sprite->setSpriteScale(glm::vec2(0.5f, 0.5f));
	sprite->setSpritePos(spawnPos);

	if (type == WeaponType::Bullet)
	{
		cBullet *bullet = new cBullet(owner, damage);
		bullet->SetSprite(sprite);
		bullet->SetRotation(angle);
		cGame::Get()->AddGameObject(bullet);
	}
	else if (type == WeaponType::Missile)
	{
		cMissile *missile = new cMissile(owner, target, damage);
		missile->SetSprite(sprite);
		missile->SetRotation(angle);
		cGame::Get()->AddGameObject(missile);
	}

	reload = reloadTimer;
}
