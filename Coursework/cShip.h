#pragma once

#include "cGameObject.h"
#include "cWeapon.h"
#include "cTrail.h"

enum class ShipType { Scout, Fighter, Corvette, Cruiser };

class cShip :
	public cGameObject
{
private:
	int fliesRight;
	glm::vec4 *weapOffsets = NULL;

	//levels
	ShipType type;
	int engineLevel;
	int hullLevel;
	int bulletLevel;
	int missileLevel;

	void AddWeapon(cWeapon *weapon) { weapons.push_back(weapon); }

protected:
	int health;
	int maxHealth;
	float maxVel;
	float accelRate;

	vector<cWeapon*> weapons;

	void Shoot(cGameObject *target);
	
public:
	cShip(ShipType pType, Owner pOwner = Owner::Enemy);
	~cShip();

	int GetHealth() { return health; }
	int GetMaxHealth() { return maxHealth; }
	
	//1100
	//MBSP - Missile, Bullet, Ship, Player
	int GetCollisionMask() { return 1 << 2 | 1 << 3; }
	int GetCollisionLayer() { return 1 << 1; }

	void Update(float delta);
	void CollidedWith(cGameObject *col);
	void ApplyDamage(int damage) { health -= damage; destroy = health <= 0; }
	
	string GetName() { return "Ship"; }
	void OnDestroy();

	//levels
	ShipType GetShipType() { return type; }
	void SetShipType(ShipType pType);
	int GetShipUpCost() { return ((int)type + 1) * ((int)type + 1) * 100; }

	int GetEngineLevel() { return engineLevel; }
	void SetEngineLevel(int lvl);
	int GetEngineUpCost() { return (engineLevel + 1) * (engineLevel + 1) * 50; }

	int GetHullLevel() { return hullLevel; }
	void SetHullLevel(int lvl);
	int GetHullUpCost() { return (hullLevel + 1) * (hullLevel + 1) * 50; }

	int GetBulletLevel() { return bulletLevel; }
	void SetBulletLevel(int lvl);
	int GetBulletUpCost() { return (bulletLevel + 1) * (bulletLevel + 1) * 50; }

	int GetMissileLevel() { return missileLevel; }
	void SetMissileLevel(int lvl);
	int GetMissileUpCost() { return (missileLevel + 1) * (missileLevel + 1) * 50; }

	void Repair() { health = maxHealth; }
	int GetFixCost() { return (maxHealth - health) / 2; }

	float GetMaxVel() { return maxVel; }
	float GetRotSpeed() { return rotSpeed; }
	float GetAccelRate() { return accelRate; }
};

