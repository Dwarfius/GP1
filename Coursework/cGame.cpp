#include "cGame.h"

cGame::cGame()
{
	texture.createTexture("ship.png");
	sprite.setTexture(texture.getTexture());
	sprite.setTextureDimensions(texture.getTWidth(), texture.getTHeight());
	sprite.setSpritePos(glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	sprite.setSpriteScale(glm::vec2(0.5f, 0.5f));
	gameObject.SetSprite(&sprite);
}

cGame::~cGame()
{
}

void cGame::HandleInput(WPARAM key)
{
	
}

void cGame::Update(float delta)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	mousePos = glm::vec2(p.x, p.y);

	gameObject.LookAt(mousePos);
	gameObject.Update(delta);
}

void cGame::Render()
{
	gameObject.Render();
}