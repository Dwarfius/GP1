#include "cMunitions.h"

void cMunitions::Update(float delta)
{
	

	currentLife += delta;
	if (currentLife > maxLife)
		destroy = true;

	//always have this!
	sprite->updateMatrix();
	sprite->updateBoundingRect();
	UpdateForward();
}