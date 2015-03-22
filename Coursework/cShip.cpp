#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

cShip::cShip(cTexture *pText, Owner pOwner) : 
	cGameObject(pText, pOwner)
{
	maxVel = 100;
	fliesRight = rand() % 3 - 1;
}

void cShip::SetStats(int pMaxHealth, float pMaxVel, float pRotSpeed)
{
	health = maxHealth = pMaxHealth;
	maxVel = pMaxVel;
	rotSpeed = pRotSpeed;
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
		LookAt(player->GetPosition());
		glm::vec2 dir = glm::normalize(deltaVec);
		if (distanceSqr <= 10000)
			dir = -dir;
		
		AddVelocity(dir * maxVel * delta);
		
		//adding random right/0/left velocity so that they don't bunch up
		//plays out better - the palyer has to deal with multiple clouds of ships
		AddVelocity(GetRight() * maxVel * delta * (float)fliesRight);

		if (distanceSqr < 10000000 && glm::abs(glm::dot(GetForward(), dir)) > 0.9f)
			Shoot(player);
	}

	velocity.x -= glm::sign(velocity.x) * 25 * delta;
	velocity.y -= glm::sign(velocity.y) * 25 * delta;

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