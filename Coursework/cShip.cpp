#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

cShip::cShip(Owner pOwner) : 
	cGameObject(pOwner)
{
	maxVel = 100;
}

void cShip::Update(float delta)
{
	if (destroy)
		return;

	cPlayer *player = cGame::Get()->GetPlayer();
	if (player && !player->IsDead())
	{
		glm::vec2 deltaVec = player->GetPosition() - GetPosition();
		float distanceSqr = deltaVec.y * deltaVec.y + deltaVec.x * deltaVec.x;
		if (distanceSqr < 1000000)
		{
			LookAt(player->GetPosition());
			glm::vec2 dir = glm::normalize(deltaVec);
			if (distanceSqr <= 10000)
				dir = -dir;

			AddVelocity(dir * maxVel * delta);

			if (glm::abs(glm::dot(GetForward(), dir)) > 0.9f)
				Shoot(player);
		}
	}

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	cGameObject::Update(delta);
}

void cShip::CollidedWith(cGameObject *col)
{

}

void cShip::Shoot(cGameObject *target)
{
	glm::vec2 spawnPos = GetPosition() + forward * 40.f;
	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
	{
		if ((*iter)->CanShoot())
			(*iter)->Shoot(owner, spawnPos, GetRotation(), target);
	}
}

void cShip::OnDestroy()
{
	cGame::Get()->AddScore(10);
}