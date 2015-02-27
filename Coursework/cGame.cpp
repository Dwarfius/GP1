#include "cGame.h"
#include "cInput.h"

cGame::cGame()
{
	gameObjects.reserve(10);
	bool b = texture.createTexture("ship.png");
	for (int i = 0; i < 10; i++)
	{
		cSprite *sprite = new cSprite();
		sprite->setTexture(texture.getTexture());
		sprite->setTextureDimensions(texture.getTWidth(), texture.getTHeight());
		sprite->setSpriteScale(glm::vec2(0.25f, 0.25f));
		cGameObject *obj = new cGameObject();
		obj->SetSprite(sprite);
		obj->SetPosition(glm::vec2(i % 2 * 300, i / 2 * 300));
		gameObjects.push_back(obj);
	}
}

cGame::~cGame()
{
	for (int i = 0; i < 10; i++)
		delete gameObjects[i];
}

void cGame::Update(float delta)
{
	cInput::Update();
	glm::vec2 mousePos = cInput::GetMousePos() - glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2) + gameObjects[0]->GetPosition();

	if (cInput::GetKey('W'))
		gameObjects[0]->AddVelocity(100 * delta);
	else if (cInput::GetKey('S'))
		gameObjects[0]->AddVelocity(-100 * delta);

	gameObjects[0]->LookAt(mousePos);
	for (int i = 0; i < 10; i++)
		gameObjects[i]->Update(delta);
}

void cGame::Render()
{
	glm::vec2 pos = gameObjects[0]->GetPosition();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(pos.x - WINDOW_WIDTH / 2, pos.x + WINDOW_WIDTH / 2,
		pos.y + WINDOW_HEIGHT / 2, pos.y - WINDOW_HEIGHT / 2, -1.f, 1.f);
	for(int i=0; i<10; i++)
		gameObjects[i]->Render();
}

bool Intersects(RECTF r1, RECTF r2)
{
	return r1.top < r2.bottom && r1.left < r2.right && r1.bottom > r2.top && r1.right > r2.left;
}

void cGame::CollisionUpdate()
{
	float left = gameObjects[0]->GetRect().left;
	float right = gameObjects[0]->GetRect().right;
	float center = (left + right) / 2.f;
	cout << to_string(left) + " + " + to_string(right) + " = " + to_string(center) << endl;

	/*for (int i = 0; i < 9; i++)
	{
		for (int j = i + 1; j < 10; j++)
		{
			if (Intersects(gameObjects[i]->GetRect(), gameObjects[j]->GetRect()))
			{
				delete gameObjects[j];
				gameObjects.erase(gameObjects.begin() + j);
			}
		}
	}*/
}