#include "cGame.h"
#include "cInput.h"
#include "cPlayer.h"
#include "cShip.h"

#pragma warning(disable: 4018 4244)

cGame* cGame::singleton;

cGame::cGame()
{
	cTexture *missile = new cTexture("missile.png");
	texture.createTexture("ship.png");

	for (int i = 0; i < 10; i++)
	{
		cSprite *sprite = new cSprite();
		sprite->setTexture(&texture);
		sprite->setSpriteScale(glm::vec2(0.25f, 0.25f));
		cShip *obj = i == 0 ? new cPlayer() : new cShip();
		obj->SetSprite(sprite);
		obj->SetPosition(glm::vec2(i % 2 * 300, i / 2 * 300));
		obj->SetMissileText(missile);
		gameObjects.push_back(obj);
	}
}

cGame::~cGame()
{
	for (int i = 0; i < gameObjects.size(); i++)
		delete gameObjects[i];
}

void cGame::Update(float delta)
{
	cInput::Update();

	for (int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Update(delta);
}

void cGame::Render()
{
	glm::vec2 pos = gameObjects[0]->GetPosition();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(pos.x - WINDOW_WIDTH / 2, pos.x + WINDOW_WIDTH / 2, 
		pos.y + WINDOW_HEIGHT / 2, pos.y - WINDOW_HEIGHT / 2, -1, 1);

	for(int i=0; i<gameObjects.size(); i++)
		gameObjects[i]->Render();
}

bool Intersects(RECTF r1, RECTF r2)
{
	return (r2.left < r1.right &&
		r2.right > r1.left &&
		r2.top > r1.bottom &&
		r2.bottom < r1.top);
}

void cGame::CollisionUpdate()
{
	for (int i = 0; i < gameObjects.size() - 1; i++)
	{
		if (gameObjects[i]->IsDead())
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
			i--;
			continue;
		}
		for (int j = i + 1; j < gameObjects.size(); j++)
		{
			if (gameObjects[j]->IsDead())
			{
				delete gameObjects[j];
				gameObjects.erase(gameObjects.begin() + j);
				j--;
				continue;
			}

			if (Intersects(gameObjects[i]->GetRect(), gameObjects[j]->GetRect()) &&
				PerPixelCollision(gameObjects[i], gameObjects[j]))
			{
				gameObjects[i]->CollidedWith(gameObjects[j]);
				gameObjects[j]->CollidedWith(gameObjects[i]);
			}
		}
	}
}

bool cGame::PerPixelCollision(cGameObject *g1, cGameObject *g2)
{
	//required data
	char *g1Data = g1->GetData();
	char *g2Data = g2->GetData();
	RECTF g1Rect = g1->GetRect();
	RECTF g2Rect = g2->GetRect();
	glm::mat4x4 g1InvTrans = glm::inverse(g1->GetTransform()); //returns pos to [-width, width], [-height, height]
	glm::mat4x4 g2InvTrans = glm::inverse(g2->GetTransform()); //need to adjust for that later on
	glm::vec2 g1FullSize = g1->GetSize(); //non-transformed size
	glm::vec2 g2FullSize = g2->GetSize();
	glm::vec2 g1HalfSize = g1FullSize * 0.5f;
	glm::vec2 g2HalfSize = g2FullSize * 0.5f;

	//rect of intersection
	int top = max(g1Rect.bottom, g2Rect.bottom);
	int bottom = min(g1Rect.top, g2Rect.top);
	int left = min(g1Rect.left, g2Rect.left);
	int right = max(g1Rect.right, g2Rect.right);

	for (int y = top; y < bottom; y++)
	{
		for (int x = left; x < right; x++)
		{
			//getting the point in model space
			glm::vec4 point = glm::vec4(x, y, 0, 1);
			glm::vec4 g1Point = g1InvTrans * point;
			glm::vec4 g2Point = g2InvTrans * point;

			//taking the color info
			glm::vec2 g1ColorP = glm::vec2((int)g1Point.x, (int)g1Point.y) + g1HalfSize;
			glm::vec2 g2ColorP = glm::vec2((int)g2Point.x, (int)g2Point.y) + g2HalfSize;

			if (g1ColorP.x < 0 || g1ColorP.x >= g1FullSize.x || g1ColorP.y < 0 || g1ColorP.y >= g1FullSize.y ||
				g2ColorP.x < 0 || g2ColorP.x >= g2FullSize.x || g2ColorP.y < 0 || g2ColorP.y >= g2FullSize.y)
				continue; //since we're using bounding rects, we can get pixels outside of texture

			int i1 = (int)(g1ColorP.y * g1FullSize.x + g1ColorP.x) * 4 + 3;
			int i2 = (int)(g2ColorP.y * g2FullSize.x + g2ColorP.x) * 4 + 3;
			if (g1Data[i1] != 0 && g2Data[i2] != 0)
				return true;
		}
	}
	return false;
}