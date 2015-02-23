#include "cGame.h"

cGame::cGame()
{
	texture.createTexture("ship.png");
	sprite.setTexture(texture.getTexture());
	sprite.setTextureDimensions(texture.getTWidth(), texture.getTHeight());
	sprite.setSpritePos(glm::vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	sprite.setSpriteScale(glm::vec2(0.5f, 0.5f));
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
	glm::vec2 mPos(p.x, p.y);

	glm::vec2 n = mPos - sprite.getSpritePos();
	float angle = glm::degrees(glm::atan(n.y, n.x));
	sprite.setSpriteRotation(angle + 90);
}

void cGame::Render()
{
	sprite.render();
}