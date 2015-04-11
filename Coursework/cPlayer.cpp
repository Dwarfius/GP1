#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

cPlayer::cPlayer(ShipType pType) : cShip(pType, Owner::Player)
{
	trail = new cTrail();
	SetBulletLevel(0);
	SetEngineLevel(0);
	SetHullLevel(0);
	SetMissileLevel(0);
}

void cPlayer::Update(float delta)
{
	if (destroy)
		return;

	trailTimer += delta;
	if (trailTimer > 0.5f)
	{
		trail->Add(GetPosition());
		trailTimer = 0;
	}

	glm::vec2 mousePos = cInput::GetMousePos() - cGame::Get()->GetWindowSize() * 0.5f + GetPosition();
	if (cInput::ControllerConnected())
	{
		if (cInput::LeftStickActive())
		{
			glm::vec2 leftStick = cInput::GetLeftStick();
			leftStick.y *= -1.f;
			AddVelocity(leftStick * accelRate * delta);
		}
		
		if (cInput::RightStickActive())
		{
			glm::vec2 rightStick = cInput::GetRightStick();
			rightStick.y *= -1.f;
			LookAt(GetPosition() + rightStick);
		}
	}
	else
	{
		if (cInput::GetKey('W'))
			AddVelocity(glm::vec2(0, -accelRate * delta));
		else if (cInput::GetKey('S'))
			AddVelocity(glm::vec2(0, accelRate * delta));

		if (cInput::GetKey('D'))
			AddVelocity(glm::vec2(accelRate * delta, 0));
		else if (cInput::GetKey('A'))
			AddVelocity(glm::vec2(-accelRate * delta, 0));

		LookAt(mousePos);
	}

	float length2 = glm::length2(velocity);
	if (length2 > maxVel * maxVel)
		velocity = glm::normalize(velocity) * maxVel;

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	if (cInput::GetButton(0) || cInput::GetRightTrigger())
	{
		cShip *ship = cGame::Get()->GetShipUnderPoint(mousePos);
		Shoot(ship);
	}

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
}