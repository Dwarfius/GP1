#pragma once

#include "cShip.h"

class cPlayer :
	public cShip
{
public:
	cPlayer();
	~cPlayer() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
	string GetName() { return "Player"; }
};

