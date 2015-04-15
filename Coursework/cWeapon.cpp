#include "cWeapon.h"
#include "cBullet.h"
#include "cMissile.h"
#include "cGame.h"

cWeapon::cWeapon(cTexture *pProjTexture, WeaponType pType, int pDamage)
{
	projTexture = pProjTexture;
	type = pType;
	damage = pDamage;
}

void cWeapon::Update(float delta)
{
	reload -= delta;
}

void cWeapon::Shoot(Owner owner, glm::vec2 spawnPos, float angle, cGameObject *target)
{
	if (type == WeaponType::Missile && target == NULL) //missiles can't shoot randomly
		return;

	cGameObject *obj = NULL;
	if (type == WeaponType::Bullet) //depending on type, create specific projectile
		obj = new cBullet(projTexture, owner, damage);
	else if (type == WeaponType::Missile)
		obj = new cMissile(projTexture, owner, target, damage);

	//position and rotate it properly
	obj->SetPosition(spawnPos);
	obj->SetRotation(angle); 
	if (type == WeaponType::Bullet)
		obj->SetVelocity(obj->GetForward() * 500.f); //give the bullet some push
	cGame::Get()->AddGameObject(obj);

	reload = reloadTimer; //now on cooldown
}
