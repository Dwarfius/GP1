#pragma once

#include "cGameObject.h"

class cShip :
	public cGameObject
{
private:
	int health = 100;
	int maxHealth = 100;
	glm::vec2 targetPos;
	float timeToReload = 1;
	cTexture *missileText;

protected:
	float reloadTimer;

	void Shoot(cGameObject *target);
	bool CanShoot() { return reloadTimer < 0; }
	
public:
	cShip() {}
	~cShip() {}

	int GetHealth() { return health; }
	int GetMaxHealth() { return maxHealth; }

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	void ApplyDamage(int damage) { health -= damage; destroy = health <= 0; }
	void SetMissileText(cTexture *text) { missileText = text; }
};

