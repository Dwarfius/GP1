#include "cShip.h"

void cShip::Update(float delta)
{
	velocity -= glm::sign(velocity) * 10 * delta;
	cGameObject::Update(delta);
}

void cShip::CollidedWith(cGameObject *col)
{

}