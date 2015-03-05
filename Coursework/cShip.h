#pragma once

#include "cGameObject.h"

class cShip :
	public cGameObject
{
private:
	int health;
	int maxHealth;
	glm::vec2 targetPos;
	float timeToReload = 1;
	cTexture *missileText;

protected:
	float reloadTimer;

	void Shoot();
	bool CanShoot() { return reloadTimer < 0; }
	
public:
	cShip() {}
	~cShip() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
	void ApplyDamage(int damage) { health -= damage; destroy = health <= 0; }
	void SetMissileText(cTexture *text) { missileText = text; }
};

