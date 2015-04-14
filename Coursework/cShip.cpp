#include "cShip.h"
#include "cGame.h"
#include "cBullet.h"
#include "cMissile.h"

#pragma warning(disable : 4018 4244)

cShip::cShip(ShipType pType, Owner pOwner) : 
	cGameObject(NULL, pOwner)
{
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
		SetHullLevel(1);
		SetMissileLevel(0);
		break;
	case ShipType::Corvette:
		SetBulletLevel(2);
		SetEngineLevel(2);
		SetHullLevel(2);
		SetMissileLevel(1);
		break;
	case ShipType::Cruiser:
		SetBulletLevel(3);
		SetEngineLevel(3);
		SetHullLevel(3);
		SetMissileLevel(1);
		break;
	default:
		break;
	}
}

cShip::~cShip()
{
	delete weapOffsets;
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
	cGame::Get()->AddScore(10  + 20 * (int)type);
}

void cShip::SetShipType(ShipType pType)
{
	type = pType;
	if (weapOffsets)
		delete weapOffsets;
	weapons.clear();
	AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), WeaponType::Bullet, 10)); //always has this
	switch (type)
	{
	case ShipType::Scout:
		sprite->setTexture(cGame::Get()->GetTexture("scout"));
		weapOffsets = new glm::vec4[1];
		weapOffsets[0] = glm::vec4(0, 10, 0, 1);
		break;
	case ShipType::Fighter:
		sprite->setTexture(cGame::Get()->GetTexture("fighter"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), WeaponType::Missile, 30));
		weapOffsets = new glm::vec4[2];
		weapOffsets[0] = glm::vec4(0, 0, 0, 1);
		weapOffsets[1] = glm::vec4(0, 0, 0, 1);
		break;
	case ShipType::Corvette:
		sprite->setTexture(cGame::Get()->GetTexture("corvette"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), WeaponType::Missile, 30));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), WeaponType::Bullet, 10));
		weapOffsets = new glm::vec4[3];
		weapOffsets[0] = glm::vec4(-30, 0, 0, 1);
		weapOffsets[1] = glm::vec4(0, 0, 0, 1);
		weapOffsets[2] = glm::vec4(30, 0, 0, 1);
		break;
	case ShipType::Cruiser:
		sprite->setTexture(cGame::Get()->GetTexture("cruiser"));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), WeaponType::Bullet, 10));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), WeaponType::Missile, 30));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("missile"), WeaponType::Missile, 30));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), WeaponType::Bullet, 10));
		AddWeapon(new cWeapon(cGame::Get()->GetTexture("bullet"), WeaponType::Bullet, 10));
		weapOffsets = new glm::vec4[6];
		weapOffsets[0] = glm::vec4(20, -5, 0, 1);
		weapOffsets[1] = glm::vec4(-20, -5, 0, 1);
		weapOffsets[2] = glm::vec4(-40, 0, 0, 1);
		weapOffsets[3] = glm::vec4(40, 0, 0, 1);
		weapOffsets[4] = glm::vec4(10, 20, 0, 1);
		weapOffsets[5] = glm::vec4(-10, 20, 0, 1);
		break;
	default:
		break;
	}
	//recalculating everything that depends on ship type
	SetEngineLevel(engineLevel);
	SetHullLevel(hullLevel);
	//since we recreate the weapons, we need to reset their levels
	SetBulletLevel(bulletLevel);
	SetMissileLevel(missileLevel);
}

void cShip::SetEngineLevel(int lvl)
{
	engineLevel = lvl;
	int shipLvl = (int)type;
	float pow2 = glm::pow(2.f, (float)shipLvl);
	maxVel = 150 + 50 * engineLevel - 25 * shipLvl;
	rotSpeed = 180 + 15 * engineLevel - 25 * pow2;
	accelRate = 150 + 50 * engineLevel - 40 * pow2;
}

void cShip::SetHullLevel(int lvl)
{
	hullLevel = lvl;
	health = maxHealth = 200 + 100 * hullLevel + 100 * (int)type;
	armor = 2 * (hullLevel + (int)type);
}

void cShip::SetBulletLevel(int lvl)
{
	bulletLevel = lvl;
	for (cWeapon *w : weapons)
	{
		if (w->GetType() == WeaponType::Bullet)
		{
			w->SetDamage(GetBulletDamage());
			w->SetReloadTimer(GetBulletCooldown());
		}
	}
}

void cShip::SetMissileLevel(int lvl)
{
	missileLevel = lvl;
	for (cWeapon *w : weapons)
	{
		if (w->GetType() == WeaponType::Missile)
		{
			w->SetDamage(GetMissileDamage());
			w->SetReloadTimer(GetMissileCooldown());
		}
	}
}

void cShip::ApplyDamage(int damage)
{
	damage -= armor;
	if (damage < 0)
		damage = 0;
	health -= damage; 
	destroy = health <= 0;
}