#include "cGame.h"
#include "cInput.h"
#include "cPlayer.h"
#include "cShip.h"

#pragma warning(disable: 4018 4244)

cGame* cGame::singleton;

cGame::cGame()
{
	LoadTextures();
	gui = new cGUI(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	background = new cBackground(textures["space"], WINDOW_WIDTH, WINDOW_HEIGHT);
}

cGame::~cGame()
{
	delete gui;
	delete background;

	for (int i = 0; i < gameObjects.size(); i++)
		delete gameObjects[i];
	for (map<string, cTexture*>::iterator it = textures.begin(); it != textures.end(); it++)
		delete it->second;
}

void cGame::Update(float delta)
{
	glm::vec2 deltaMove = glm::vec2(0, 0);
	if (!paused)
	{
		//cleaning up objects scheduled for deletion
		while (objctsToDelete.size() > 0)
		{
			objctsToDelete[0]->OnDestroy();
			delete objctsToDelete[0];
			objctsToDelete.erase(objctsToDelete.begin());
		}

		if (player)
		{
			glm::vec2 pos = player->GetPosition();
			for (int i = 0; i < gameObjects.size(); i++)
				gameObjects[i]->Update(delta);
			deltaMove = player->GetPosition() - pos;

			if (player->IsDead())
			{
				Clear();
				gui->SetMenu(0);
			}
		}
	}

	background->Update(deltaMove);
	gui->Update(delta);
	cInput::Update();
}

void cGame::Render()
{
	background->Render();

	glm::vec2 pos = player ? player->GetPosition() : glm::vec2(0, 0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(pos.x - WINDOW_WIDTH / 2, pos.x + WINDOW_WIDTH / 2, 
		pos.y + WINDOW_HEIGHT / 2, pos.y - WINDOW_HEIGHT / 2, -1, 1);

	for(int i=0; i<gameObjects.size(); i++)
		gameObjects[i]->Render();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	gui->Render(pos);
}

void cGame::CollisionUpdate()
{
	if (gameObjects.size() == 0)
		return;

	for (int i = 0; i < gameObjects.size() - 1; i++)
	{
		if (gameObjects[i]->IsDead())
		{
			objctsToDelete.push_back(gameObjects[i]);
			gameObjects.erase(gameObjects.begin() + i);
			i--;
			continue;
		}
		for (int j = i + 1; j < gameObjects.size(); j++)
		{
			if (gameObjects[j]->IsDead())
			{
				objctsToDelete.push_back(gameObjects[j]);
				gameObjects.erase(gameObjects.begin() + j);
				j--;
				continue;
			}
			
			if (gameObjects[i]->GetOwner() != gameObjects[j]->GetOwner() && 
				RECTF::Intersects(gameObjects[i]->GetRect(), gameObjects[j]->GetRect()) &&
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
	int bottom = max(g1Rect.bottom, g2Rect.bottom);
	int top = min(g1Rect.top, g2Rect.top);
	int left = min(g1Rect.left, g2Rect.left);
	int right = max(g1Rect.right, g2Rect.right);

	for (int y = top; y < bottom; y++) //hello invcerted Y axis
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
				continue; //since we're using AABB, we can get pixels outside of texture

			int i1 = (int)(g1ColorP.y * g1FullSize.x + g1ColorP.x) * 4 + 3;
			int i2 = (int)(g2ColorP.y * g2FullSize.x + g2ColorP.x) * 4 + 3;
			if (g1Data[i1] != 0 && g2Data[i2] != 0)
				return true;
		}
	}
	return false;
}

cGameObject* cGame::ClickedOn(glm::vec2 pos)
{
	cGameObject *obj = NULL;
	for (vector<cGameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if (RECTF::InRect((*iter)->GetRect(), pos))
			obj = *iter;
	}
	return obj;
}

void cGame::StartLevel(int level)
{
	if (level == 0)
		cInput::Reset();

	for (int i = 0; i < 3; i++)
	{
		cSprite *sprite = new cSprite();
		sprite->setTexture(textures["ship"]);
		sprite->setSpriteScale(glm::vec2(0.25f, 0.25f));

		cShip *ship;
		if (level == 0 && i == 0)
		{
			player = new cPlayer();
			ship = player;
		}
		else
			ship = new cShip();
		ship->AddWeapon(new cWeapon(textures["bullet"], 0.5f, WeaponType::Bullet, 10));
		ship->AddWeapon(new cWeapon(textures["missile"], 2, WeaponType::Missile, 30));
		ship->SetSprite(sprite);
		ship->SetPosition(glm::vec2(i % 2 * 300, i / 2 * 300));
		gameObjects.push_back(ship);
	}
}

void cGame::LoadTextures()
{
	textures.insert(pair<string, cTexture*>("ship", new cTexture("ship.png")));
	textures.insert(pair<string, cTexture*>("missile", new cTexture("missile.png")));
	textures.insert(pair<string, cTexture*>("bullet", new cTexture("bullet.png")));
	textures.insert(pair<string, cTexture*>("space", new cTexture("space.png")));
}

void cGame::Clear()
{
	score = 0;
	player = NULL;
	paused = false;

	while (gameObjects.size() > 0)
	{
		delete gameObjects[0];
		gameObjects.erase(gameObjects.begin());
	}
	
	while (objctsToDelete.size() > 0)
	{
		delete objctsToDelete[0];
		objctsToDelete.erase(objctsToDelete.begin());
	}
}

void cGame::OnResize(int width, int height)
{
	gui->UpdateSize(glm::vec2(width, height));
	background->UpdateSize(width, height);
}