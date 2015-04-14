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
	cWeapon(cTexture *pProjTexture, WeaponType pType, int pDamage);
	~cWeapon() {}

	void Update(float delta);
	void Shoot(Owner owner, glm::vec2 spawnPos, float angle, cGameObject *target);
	bool CanShoot() { return reload < 0; }

	float GetReloadTimer() { return reloadTimer; }
	void SetReloadTimer(float val) { reloadTimer = val; }

	void SetDamage(int pDamage) { damage = pDamage; }

	WeaponType GetType() { return type; }
};

