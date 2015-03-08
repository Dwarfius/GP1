#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

void cPlayer::Update(float delta)
{
	if (destroy)
		return;

	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f + GetPosition();
	LookAt(mousePos);

	if (cInput::GetKey('W'))
		AddVelocity(glm::vec2(400 * delta, 0));
	else if (cInput::GetKey('S'))
		AddVelocity(glm::vec2(-400 * delta, 0));
	if (cInput::GetKey('D'))
		AddVelocity(glm::vec2(0, -400 * delta));
	else if (cInput::GetKey('A'))
		AddVelocity(glm::vec2(0, 400 * delta));

	velocity.x -= glm::sign(velocity.x) * 10 * delta; //dampening of 10m/s
	velocity.y -= glm::sign(velocity.y) * 10 * delta;

	for (auto iter = weapons.begin(); iter != weapons.end(); iter++)
		(*iter)->Update(delta);

	if (cInput::GetButton(0))
	{
		cGameObject *obj = cGame::Get()->ClickedOn(mousePos);
		Shoot((cShip*)obj);
	}

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
}