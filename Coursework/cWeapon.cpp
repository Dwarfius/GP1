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

	cGameObject *obj = NULL;
	if (type == WeaponType::Bullet)
		obj = new cBullet(projTexture, owner, damage);
	else if (type == WeaponType::Missile)
		obj = new cMissile(projTexture, owner, target, damage);

	obj->SetPosition(spawnPos);
	obj->SetRotation(angle);
	if (type == WeaponType::Bullet)
		obj->SetVelocity(obj->GetForward() * 500.f);
	cGame::Get()->AddGameObject(obj);

	reload = reloadTimer;
}
