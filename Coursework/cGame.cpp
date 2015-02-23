#include "cGame.h"

cGame::cGame()
{
	bool test = texture.createTexture("b1fv.png");
	sprite.setTexture(texture.getTexture());
	sprite.setTextureDimensions(texture.getTWidth(), texture.getTHeight());
	sprite.setSpritePos(glm::vec2(100, 100));
}

cGame::~cGame()
{
	
}

void cGame::HandleInput(WPARAM key)
{

}

void cGame::Update(float delta)
{
	float rotation = sprite.getSpriteRotation() + delta * 50;
	sprite.setSpriteRotation(rotation);
	sprite.updateMatrix();
}

void cGame::Render()
{
	sprite.render();
}