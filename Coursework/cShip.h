#pragma once

#include "cGameObject.h"

class cShip :
	public cGameObject
{
private:
	int health;
	int maxHealth;
	glm::vec2 targetPos;

public:
	cShip() {}
	~cShip() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
	void ApplyDamage(int damage) { health -= damage; destroy = health <= 0; }
};

