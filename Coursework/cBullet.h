#pragma once

#include "cGameObject.h"

class cBullet :
	public cGameObject
{
private:
	float currentLife;
	float maxLife = 15;
	int damage;

public:
	cBullet(int pDamage) { damage = pDamage; }
	~cBullet() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
};

