#include "cGame.h"

cGame::cGame()
{
	gameObjects.reserve(10);
	texture.createTexture("ship.png");
	for (int i = 0; i < 10; i++)
	{
		cSprite *sprite = new cSprite();
		sprite->setTexture(texture.getTexture());
		sprite->setTextureDimensions(texture.getTWidth(), texture.getTHeight());
		sprite->setSpriteScale(glm::vec2(0.25f, 0.25f));
		cGameObject obj;
		obj.SetSprite(sprite);
		obj.SetPosition(glm::vec2(i % 4 * 300, i / 5 * 300));
	}
}

cGame::~cGame()
{
}

void cGame::HandleInput(WPARAM key)
{
	if (key == 'W')
		gameObjects[0].AddVelocity(10);
	else if (key == 'S')
		gameObjects[0].AddVelocity(-10);
}

void cGame::Update(float delta)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	mousePos = glm::vec2(p.x - WINDOW_WIDTH / 2, p.y - WINDOW_HEIGHT / 2) + gameObjects[0].GetPosition();

	gameObjects[0].LookAt(mousePos);
	for (int i = 0; i < 10; i++)
		gameObjects[i].Update(delta);
}

void cGame::Render()
{
	glm::vec2 pos = gameObjects[0].GetPosition();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(pos.x - WINDOW_WIDTH / 2, pos.x + WINDOW_WIDTH / 2,
		pos.y + WINDOW_HEIGHT / 2, pos.y - WINDOW_HEIGHT / 2, -1.f, 1.f);
	for(int i=0; i<10; i++)
		gameObjects[i].Render();
}