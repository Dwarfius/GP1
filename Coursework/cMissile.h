#pragma once

#include "cGameObject.h"

class cMissile :
	public cGameObject
{
private:
	float currentLife = 15;
	int damage;
	int t = 0;
	cGameObject *target; //fun fact, if it's the first member (before currentLife), somehow the value gets corrupted on collision with cBullet

public:
	cMissile(cGameObject *pTarget, int pDamage);
	~cMissile() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Missile"; }
};

