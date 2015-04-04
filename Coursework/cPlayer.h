#pragma once

#include "cShip.h"

class cPlayer :
	public cShip
{
public:
	cPlayer(ShipType pType);
	~cPlayer() {}

	void Update(float delta);
	void CollidedWith(cGameObject *col);

	string GetName() { return "Player"; }

	//1100
	//MBSP - Missile, Bullet, Ship, Player
	int GetCollisionMask() { return 1 << 2 | 1 << 3; }
	int GetCollisionLayer() { return 1; }
};

