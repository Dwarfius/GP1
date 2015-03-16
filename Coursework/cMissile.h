#pragma once

#include "cGameObject.h"

class cMissile :
	public cGameObject
{
private:
	cGameObject *target;
	float currentLife = 15;
	int damage;

public:
	cMissile(cTexture *pText, Owner pOwner, cGameObject *pTarget, int pDamage);
	~cMissile() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Missile"; }

	//0111
	//MBSP - Missile, Bullet, Ship, Player
	int GetCollisionMask() { return 1 || 1 << 1 || 1 << 2; }
};

