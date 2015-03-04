#pragma once

#include "cGameObject.h"

enum class eMunitionsType { Bullet = 1, Missile = 2 };

class cMunitions :
	public cGameObject
{
private:
	eMunitionsType munType;
	float currentLife;
	float maxLife = 15;

public:
	cMunitions() { munType = eMunitionsType::Bullet; }
	cMunitions(eMunitionsType type) { munType = type; }
	~cMunitions() {}
	void Update(float delta);
};

