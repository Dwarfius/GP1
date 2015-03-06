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
	cMissile(cGameObject *pTarget, int pDmg);
	~cMissile() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
};

