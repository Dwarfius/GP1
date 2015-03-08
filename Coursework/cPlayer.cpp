#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

void cPlayer::Update(float delta)
{
	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f + GetPosition();
	LookAt(mousePos);

	if (cInput::GetKey('W'))
		AddVelocity(forward * 400.f * delta);
	else if (cInput::GetKey('S'))
		AddVelocity(forward * -400.f * delta);
	if (cInput::GetKey('D'))
		AddVelocity(GetRight() * -400.f * delta);
	else if (cInput::GetKey('A'))
		AddVelocity(GetRight() * 400.f * delta);

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	reloadTimer -= delta;
	if (cInput::GetButtonDown(0) && CanShoot())
	{
		cGameObject *obj = cGame::Get()->ClickedOn(mousePos);
		if (cShip *target = (cShip*)obj)
			Shoot(target);
	}

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
	cout << "Player colided with " << typeid(col).name() << endl;
}