#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

cPlayer::cPlayer(cTexture *pText) : cShip(pText, Owner::Player)
{
	
}

void cPlayer::Update(float delta)
{
	if (destroy)
		return;

	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f + GetPosition();
	LookAt(mousePos);

	if (cInput::GetKey('W'))
		AddVelocity(GetForward() * 400.f * delta);
	else if (cInput::GetKey('S'))
		AddVelocity(GetForward() * -400.f * delta);
	if (cInput::GetKey('D'))
		AddVelocity(GetRight() * -400.f * delta);
	else if (cInput::GetKey('A'))
		AddVelocity(GetRight() * 400.f * delta);

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	if (cInput::GetButton(0))
	{
		cGameObject *obj = cGame::Get()->ClickedOn(mousePos);
		Shoot(dynamic_cast<cShip*>(obj));
	}

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
}