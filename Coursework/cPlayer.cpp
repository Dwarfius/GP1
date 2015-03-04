#include "cPlayer.h"
#include "cInput.h"

void cPlayer::Update(float delta)
{
	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f + GetPosition();
	LookAt(mousePos);

	if (cInput::GetKey('W'))
		AddVelocity(40);
	else if (cInput::GetKey('S'))
		AddVelocity(-40);
	AddVelocity(glm::sign(velocity) * -10 * delta);

	cGameObject::Update(delta);
}

void cPlayer::CollidedWith(cGameObject *col)
{
	cout << "Player colided with " << typeid(col).name() << endl;
}