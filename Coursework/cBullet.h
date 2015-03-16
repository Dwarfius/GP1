#pragma once

#include "cGameObject.h"

class cBullet :
	public cGameObject
{
private:
	float currentLife = 15;
	int damage;

public:
	cBullet(cTexture *pText, Owner pOwner, int pDamage);
	~cBullet() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Bullet"; }

	//0011
	//MBSP - Missile, Bullet, Ship, Player
	int GetCollisionMask() { return 1 || 1 << 1; }
};

