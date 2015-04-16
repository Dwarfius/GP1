#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

cPlayer::cPlayer(ShipType pType) : cShip(pType, Owner::Player)
{
	SetBulletLevel(0);
	SetEngineLevel(0);
	SetHullLevel(0);
	SetMissileLevel(0);
}

cPlayer::~cPlayer()
{
}

void cPlayer::Update(float delta)
{
	if (destroy)
		return;

	if (cInput::ControllerConnected()) //it's either controller or keyboard-mouse
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

		//transforming to local world coordinates
		glm::vec2 mousePos = cInput::GetMousePos() - cGame::Get()->GetWindowSize() * 0.5f + GetPosition();
		LookAt(mousePos);
	}

	//limiting the velocity to maximum
	float length2 = glm::length2(velocity);
	if (length2 > maxVel * maxVel)
		velocity = glm::normalize(velocity) * maxVel;

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (cWeapon *weap : weapons)
		weap->Update(delta);

	//update closest enemy
	if (!target || target->IsDead() || cInput::GetKeyDown(9) || cInput::GetControllerKeyDown(GamepadKeys::LBumper))
		target = cGame::Get()->GetNearestShip(GetPosition());

	if (cInput::GetButton(0) || cInput::GetRightTrigger())
		Shoot(target);

	CheatHandling();

	//do the basic routine
	UpdateForward();
	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
}

void cPlayer::CheatHandling()
{
	if (cInput::GetKeyDown('K'))
		cGame::Get()->AddScore(10000);
	if (cInput::GetKeyDown('L'))
		armor += armor < 10000 ? 10000 : -10000;
}