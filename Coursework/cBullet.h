#pragma once

#include "cGameObject.h"

class cBullet :
	public cGameObject
{
private:
	float currentLife = 15;
	int damage;

public:
	cBullet(int pDamage);
	~cBullet() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Bullet"; }
};

