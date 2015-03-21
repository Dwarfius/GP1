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
	//tree = new cQuadtree(0, { -10000, -10000, 10000, 10000 });
	grid = new cGrid(WINDOW_WIDTH * 3 / 2, WINDOW_HEIGHT * 3 / 2);
}

cGame::~cGame()
{
	delete gui;
	delete background;
	//delete tree;
	delete grid;

	for (int i = 0; i < gameObjects.size(); i++)
		delete gameObjects[i];
	for (auto it = textures.begin(); it != textures.end(); it++)
		delete it->second;
}

void cGame::Update(float delta)
{
	glm::vec2 deltaMove = glm::vec2(0, 0);

	if (cInput::GetKeyDown(32))
		paused = !paused; //figure out the strange bug

	if (!paused)
	{
		//cleaning up objects scheduled for deletion
		int size = objctsToDelete.size();
		while (size-- > 0)
		{
			objctsToDelete[0]->OnDestroy();
			delete objctsToDelete[0];
			objctsToDelete.erase(objctsToDelete.begin());
		}

		if (player)
		{
			glm::vec2 pos = player->GetPosition();
			int enemiesCount = 0;
			//start thinking about multithreading this
			//http://gamedev.stackexchange.com/questions/7338/multi-threaded-games-best-practices-one-thread-for-logic-one-for-rendering
			for (int i = 0; i < gameObjCount; i++)
			{
				cGameObject *obj = gameObjects[i];
				cShip *ship = dynamic_cast<cShip*>(obj);
				if (ship &&	ship->GetOwner() == Owner::Enemy)
					enemiesCount++;
					
				obj->Update(delta);
				if (obj->IsDead())
				{
					objctsToDelete.push_back(obj);
					gameObjects.erase(gameObjects.begin() + i);
					i--;
					gameObjCount--;

					if (ship)
					{
						int shipsCount = ships.size();
						for (int j = 0; j < shipsCount; j++)
						{
							if (ships[j] == ship)
							{
								ships.erase(ships.begin() + j);
								break;
							}
						}
					}
				}
			}
				
			deltaMove = player->GetPosition() - pos;

			if (player->IsDead())
			{
				gui->SetFinalScore(score);
				gui->SetMenu(Screen::Death);
				Clear();
			}
			else if (enemiesCount == 0)
				StartLevel(++currentLevel);
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
	float vel = player ? glm::length(player->GetVelocity()) : 0;
	vel /= 3; //just for the time being, need to change it up in the future
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(pos.x - WINDOW_WIDTH / 2 - vel, pos.x + WINDOW_WIDTH / 2 + vel, 
		pos.y + WINDOW_HEIGHT / 2 + vel, pos.y - WINDOW_HEIGHT / 2 - vel, -1, 1);

	for (int i = 0; i<gameObjCount; i++)
		gameObjects[i]->Render();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	gui->Render(pos);
}

void cGame::CollisionUpdate()
{
	if (gameObjCount == 0)
		return;

	//repopulating the tree before checking for collisions
	//tree->Clear();
	grid->Clear();
	grid->SetPos(player->GetPosition());
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		cGameObject *obj = (*iter);
		if (!obj->IsDead())
			//tree->Insert(obj);
			grid->Add(obj);
	}

	//start thinking about multithreading this
	//http://gamedev.stackexchange.com/questions/7338/multi-threaded-games-best-practices-one-thread-for-logic-one-for-rendering
	for (auto iter1 = gameObjects.begin(); iter1 != gameObjects.end(); iter1++)
	{
		cGameObject *obj1 = (*iter1);
		if (obj1->IsDead()) //since objects remain in game for one extra frame, we can skip them
			continue;
		
		int colMask = obj1->GetCollisionMask();
		vector<cGameObject*> cols;
		//tree->Get(cols, obj1->GetRect());
		grid->Get(cols, obj1->GetRect());
		for (auto iter2 = cols.begin(); iter2 != cols.end(); iter2++)
		{
			cGameObject *obj2 = (*iter2);
			if (obj1 == obj2 || obj2->IsDead())
				continue;
			
			int colLayer = obj2->GetCollisionLayer();
			if ((colLayer & colMask) > 0 && //are the object types supposed to collide with each other
				obj1->GetOwner() != obj2->GetOwner() && //friendlies don't collide
				RECTF::Intersects(obj1->GetRect(), obj2->GetRect()) && //raw AABB intersection
				PerPixelCollision(obj1, obj2)) //final perpixel check
			{
				obj1->CollidedWith(obj2);
				break; //can collide only once
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

	for (int y = top; y < bottom; y++) //hello inverted Y axis
	{
		for (int x = left; x < right; x++)
		{
			//Going separate it in 2 parts, part of optimization - if one of them is transparent, why
			//check the other? So
			//getting the point in model space
			glm::vec4 point = glm::vec4(x, y, 0, 1);
			glm::vec4 g1Point = g1InvTrans * point;

			//taking the color info
			glm::vec2 g1ColorP = glm::vec2((int)g1Point.x, (int)g1Point.y) + g1HalfSize;

			//since we're using AABB, we can get pixels outside of texture, so check against that
			if (!(g1ColorP.x >= 0 && g1ColorP.x < g1FullSize.x && g1ColorP.y >= 0 && g1ColorP.y < g1FullSize.y))
				continue; 

			//get the alpha byte
			int i1 = (int)(g1ColorP.y * g1FullSize.x + g1ColorP.x) * 4 + 3;
			if (g1Data[i1] == 0)
				continue;

			//now, repeating it for second object
			glm::vec4 g2Point = g2InvTrans * point;
			glm::vec2 g2ColorP = glm::vec2((int)g2Point.x, (int)g2Point.y) + g2HalfSize;
			if (!(g2ColorP.x >= 0 && g2ColorP.x < g2FullSize.x && g2ColorP.y >= 0 && g2ColorP.y < g2FullSize.y))
				continue;
			int i2 = (int)(g2ColorP.y * g2FullSize.x + g2ColorP.x) * 4 + 3;
			if (g2Data[i2] == 0)
				continue;

			return true;
		}
	}
	return false;
}

void cGame::StartLevel(int level)
{
	currentLevel = level;
	if (level == 0)
	{
		cInput::Reset(); //just so that there's no shooting after clicking Play

		player = new cPlayer(textures["ship"]);
		player->SetStats(5 * LVL1_HEALTH, 150, 100);
		player->AddWeapon(new cWeapon(textures["bullet"], 0.5f, WeaponType::Bullet, 10));
		player->AddWeapon(new cWeapon(textures["missile"], 2, WeaponType::Missile, 30));
		gameObjects.push_back(player);
		gameObjCount++;
	}

	int count = pow(2, level);
	glm::vec2 playerPos = player->GetPosition();
	for (int i = 0; i < count; i++)
	{
		cShip *ship = new cShip(textures["ship"]);
		ship->SetStats(LVL1_HEALTH, 100, 100);
		glm::vec2 offset = glm::vec2(rand() % 800 - 400, rand() % 800 - 400);
		ship->SetPosition(playerPos + offset);
		ship->AddWeapon(new cWeapon(textures["bullet"], 0.5f, WeaponType::Bullet, 10));
		gameObjects.push_back(ship);
		ships.push_back(ship);
	}
	gameObjCount += count;
}

void cGame::LoadTextures()
{
	textures.insert(make_pair("ship", new cTexture("Textures\\Ships\\ship.png")));
	textures.insert(make_pair("missile", new cTexture("Textures\\Weapons\\missile.png")));
	textures.insert(make_pair("bullet", new cTexture("Textures\\Weapons\\bullet.png")));
	int randomBg = rand() % 6;
	string bg = "Textures\\Backgrounds\\space" + to_string(randomBg) + ".png";
	textures.insert(make_pair("space", new cTexture(bg.c_str())));
}

void cGame::LoadSounds()
{

}

void cGame::Clear()
{
	score = 0;
	player = NULL;
	paused = false;
	gameObjCount = 0;
	//tree->Clear();
	grid->Clear();
	ships.clear();

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

cShip* cGame::GetShipUnderPoint(glm::vec2 pos)
{
	for (auto iter = ships.begin(); iter != ships.end(); iter++)
	{
		cShip *ship = (*iter);
		if (!ship->IsDead() && RECTF::InRect(ship->GetRect(), pos))
			return ship;
	}
	return NULL;
}