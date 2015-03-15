#pragma once

#include "cGameObject.h"
#include "cWeapon.h"

class cShip :
	public cGameObject
{
private:
	int health = 1000;
	int maxHealth = 1000;
	glm::vec2 targetPos;

protected:
	vector<cWeapon*> weapons;

	void Shoot(cGameObject *target);
	
public:
	cShip(cTexture *pText, Owner pOwner = Owner::Enemy);
	~cShip() {}

	void SetStats(int pMaxHealth, float pMaxVel, float pRotSpeed);

	int GetHealth() { return health; }
	int GetMaxHealth() { return maxHealth; }

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	void ApplyDamage(int damage) { health -= damage; destroy = health <= 0; }
	void AddWeapon(cWeapon *weapon) { weapons.push_back(weapon); }
	string GetName() { return "Ship"; }
	void OnDestroy();
};

