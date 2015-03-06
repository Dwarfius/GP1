#include "cPlayer.h"
#include "cInput.h"
#include "cGame.h"

void cPlayer::Update(float delta)
{
	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f + GetPosition();
	LookAt(mousePos);

	if (cInput::GetKey('W'))
		AddVelocity(400 * delta);
	else if (cInput::GetKey('S'))
		AddVelocity(-400 * delta);
	AddVelocity(glm::sign(velocity) * -10 * delta);
	reloadTimer -= delta;

	if (cInput::GetButton(0) && CanShoot())
	{
		cGameObject *obj = cGame::Get()->ClickedOn(mousePos);
		cout << "Found: " << obj << endl;
		if (cShip *target = (cShip*)obj)
			Shoot(target);
	}

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
	cout << "Player colided with " << typeid(col).name() << endl;
}