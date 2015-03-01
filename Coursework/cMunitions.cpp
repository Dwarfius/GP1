#include "cMunitions.h"

void cMunitions::Update(float delta)
{
	currentLife += delta;
	if (currentLife > maxLife)
		destroy = true;
}