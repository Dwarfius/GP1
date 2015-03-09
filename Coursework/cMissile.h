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
	cMissile(Owner pOwner, cGameObject *pTarget, int pDamage);
	~cMissile() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Missile"; }
};

