#pragma once

#include "cTexture.h"
#include "cGameObject.h"

enum class WeaponType { Bullet, Missile };

class cWeapon
{
private:
	cTexture *projTexture;
	float reload = 0;
	float reloadTimer;
	WeaponType type;
	int damage;

public:
	cWeapon(cTexture *pProjTexture, float pReloadTimer, WeaponType pType, int pDamage);
	~cWeapon() {}

	void Update(float delta);
	void Shoot(Owner owner, glm::vec2 spawnPos, float angle, cGameObject *target);
	bool CanShoot() { return reload < 0; }

	void SetDamage(int pDamage) { damage = pDamage; }

	WeaponType GetType() { return type; }
};

