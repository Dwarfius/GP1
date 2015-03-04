#pragma once

#include "cShip.h"

class cPlayer :
	public cShip
{
private:

public:
	cPlayer() {}
	~cPlayer() {}
	void Update(float delta);
	void CollidedWith(cGameObject *col);
};

