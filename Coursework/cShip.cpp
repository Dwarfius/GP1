#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

#pragma warning(disable : 4018 4244)

cShip::cShip(ShipType pType, Owner pOwner) : 
	cGameObject(NULL, pOwner)
{
	//trail = new cTrail();
	fliesRight = rand() % 3 - 1;

	SetShipType(pType);
	switch (type)
	{
	case ShipType::Scout:
		SetBulletLevel(0);
		SetEngineLevel(0);
		SetHullLevel(0);
		SetMissileLevel(0);
		break;
	case ShipType::Fighter:
		SetBulletLevel(1);
		SetEngineLevel(1);
		SetHullLevel(2);
		SetMissileLevel(0);
		break;
	case ShipType::Corvette:
		SetBulletLevel(2);
		SetEngineLevel(2);
		SetHullLevel(3);
		SetMissileLevel(2);
		break;
	case ShipType::Cruiser:
		SetBulletLevel(3);
		SetEngineLevel(4);
		SetHullLevel(5);
		SetMissileLevel(3);
		break;
	default:
		break;
	}
}

cShip::~cShip()
{
	if (trail)
		delete trail;
	delete weapOffsets;
}

void cShip::Update(float delta)
{
	if (destroy)
		return;

	if (trail)
	{
		trailTimer += delta;
		if (trailTimer > 0.5f)
		{
			trail->Add(GetPosition());
			trailTimer = 0;
		}
	}

	cPlayer *player = cGame::Get()->GetPlayer();
	if (player && !player->IsDead())
	{
		glm::vec2 deltaVec = player->GetPosition() - GetPosition();
		float distanceSqr = deltaVec.y * deltaVec.y + deltaVec.x * deltaVec.x;
		LookAt(player->GetPosition());
		glm::vec2 dir = glm::normalize(deltaVec);
		if (distanceSqr <= 10000)
			dir = -dir;
		
		AddVelocity(dir * accelRate * delta);
		
		//adding random right/0/left velocity so that they don't bunch up
		//plays out better - the player has to deal with multiple clouds of ships
		AddVelocity(GetRight() * accelRate * delta * (float)fliesRight);

		float length2 = glm::length2(velocity);
		if (length2 > maxVel * maxVel)
			velocity = glm::normalize(velocity) * maxVel;

		if (distanceSqr < 10000000 && glm::abs(glm::dot(GetForward(), dir)) > 0.9f)
			Shoot(player);
	}

	velocity.x -= glm::sign(velocity.x) * 25 * delta;
	velocity.y -= glm::sign(velocity.y) * 25 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	cGameObject::Update(delta);
}

void cShip::Render()
{
	if (trail)
		trail->Render();
	sprite->render();
}

void cShip::CollidedWith(cGameObject *col)
{

}

void cShip::Shoot(cGameObject *target)
{
	for (int i = 0; i < weapons.size(); i++)
	{
		if (weapons[i]->CanShoot())
		{
			glm::vec4 spawnPos = GetTransform() * weapOffsets[i];
			weapons[i]->Shoot(owner, glm::vec2(spawnPos.x, spawnPos.y), GetRotation(), target);
		}
	}
}

void cShip::OnDestroy()
{
	cGame::Get()->AddScore(10);
}

void cShip::SetShipType(ShipType pType)
{
	type = pType;
	if (weapOffsets)
		delete weapOffsets;
	weapons.clear();
	AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), 0.5f, WeaponType::Bullet, 10)); //always has this
	switch (type)
	{
	case ShipType::Scout:
		sprite->setTexture(cGame::Get()->GetTexture("scout"));
		weapOffsets = new glm::vec4[1];
		weapOffsets[0] = glm::vec4(0, 10, 0, 1);
		break;
	case ShipType::Fighter:
		sprite->setTexture(cGame::Get()->GetTexture("fighter"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), 1.5, WeaponType::Missile, 30));
		weapOffsets = new glm::vec4[2];
		weapOffsets[0] = glm::vec4(0, 0, 0, 1);
		weapOffsets[1] = glm::vec4(0, 0, 0, 1);
		break;
	case ShipType::Corvette:
		sprite->setTexture(cGame::Get()->GetTexture("corvette"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), 1.5, WeaponType::Missile, 30));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), 0.5f, WeaponType::Bullet, 10));
		weapOffsets = new glm::vec4[3];
		weapOffsets[0] = glm::vec4(-10, 0, 0, 1);
		weapOffsets[1] = glm::vec4(0, 0, 0, 1);
		weapOffsets[2] = glm::vec4(10, 0, 0, 1);
		break;
	case ShipType::Cruiser:
		sprite->setTexture(cGame::Get()->GetTexture("cruiser"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), 1.5, WeaponType::Missile, 30));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), 0.5f, WeaponType::Bullet, 10));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), 0.5f, WeaponType::Bullet, 10));
		weapOffsets = new glm::vec4[4];
		weapOffsets[0] = glm::vec4(-10, 0, 0, 1);
		weapOffsets[1] = glm::vec4(0, 0, 0, 1);
		weapOffsets[2] = glm::vec4(10, 0, 0, 1);
		weapOffsets[2] = glm::vec4(0, -10, 0, 1);
		break;
	default:
		break;
	}
}

void cShip::SetEngineLevel(int lvl)
{
	engineLevel = lvl;
	maxVel = 100 + 25 * engineLevel;
	rotSpeed = 100 + 10 * engineLevel;
	accelRate = 200 + 50 * engineLevel;
}

void cShip::SetHullLevel(int lvl)
{
	hullLevel = lvl;
	health = maxHealth = 200 + 100 * hullLevel;
}

void cShip::SetBulletLevel(int lvl)
{
	bulletLevel = lvl;
	for (cWeapon *w : weapons)
		if (w->GetType() == WeaponType::Bullet)
			w->SetDamage(10 + 10 * bulletLevel);
}

void cShip::SetMissileLevel(int lvl)
{
	missileLevel = lvl;
	for (cWeapon *w : weapons)
		if (w->GetType() == WeaponType::Missile)
			w->SetDamage(20 + 20 * missileLevel);
}