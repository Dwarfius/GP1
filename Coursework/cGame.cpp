#include "cGame.h"
#include "cInput.h"
#include "cPlayer.h"
#include "cShip.h"
#include "cSettings.h"
#include "cTrail.h"

#pragma warning(disable: 4018 4244)

cGame* cGame::singleton;

//put all loading/initialization here
cGame::cGame()
{
	cTrail::GetFunctions();

	LoadTextures();
	gui = new cGUI(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), textures["tutorial"]);
	background = new cBackground(textures["space"], WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
	//tree = new cQuadtree(0, { -10000, -10000, 10000, 10000 });
	grid = new cGrid(WINDOW_WIDTH * 3 / 2, WINDOW_HEIGHT * 3 / 2);
	soundMgr = new cSoundMgr();
	LoadSounds();

	PlaySnd("theme", true);
}

//destructor - have cleanup here
cGame::~cGame()
{
	delete gui;
	delete background;
	//delete tree;
	delete grid;
	delete soundMgr;

	for (int i = 0; i < gameObjects.size(); i++)
		delete gameObjects[i];
	for (auto it = textures.begin(); it != textures.end(); it++)
		delete it->second;
}

//global update method
void cGame::Update(float delta)
{
	glm::vec2 deltaMove = glm::vec2(0, 0);

	if (!paused)
	{
		//cleaning up objects scheduled for deletion
		int size = objctsToDelete.size();
		while (size-- > 0) //clear up the objects
		{
			objctsToDelete[0]->OnDestroy();
			delete objctsToDelete[0];
			objctsToDelete.erase(objctsToDelete.begin());
		}

		if (player) //since everything depends on player - do it only then
		{
			glm::vec2 pos = player->GetPosition();
			int enemiesCount = 0;
			for (int i = 0; i < gameObjCount; i++)
			{
				cGameObject *obj = gameObjects[i];
				cShip *ship = dynamic_cast<cShip*>(obj);
				if (ship &&	ship->GetOwner() == Owner::Enemy)
					enemiesCount++;
					
				obj->Update(delta);
				if (obj->IsDead()) //removing scheduled for deletion objects
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
				
			//calculate the offset for paralax background
			deltaMove = player->GetPosition() - pos;

			if (player->IsDead()) //game lost
			{
				cSettings::Get()->AddScore(score);
				gui->SetFinalScore(score);
				gui->SetMenu(Screen::Death);
				Clear();
			}
			else if (enemiesCount == 0) //wave cleared
			{
				AddScore(100);
				gui->SetMenu(Screen::Upgrade);
				paused = true;
			}
		}
	}

	//update all the utilities
	background->Update(deltaMove);
	gui->Update(delta);
	cInput::Update(delta);
}

//global render
void cGame::Render()
{
	background->Render();

	//add a bit of zoom-out when flying fast
	glm::vec2 pos = player ? player->GetPosition() : glm::vec2(0, 0);
	float vel = player ? glm::length(player->GetVelocity()) : 0;
	//instead of proper resizing I do upscale
	//also, depending on ship size I zoom out more
	glm::vec2 size = player ? player->GetSize() : glm::vec2(0, 0);
	float extraSize = glm::max(size.x, size.y);
	float width = WINDOW_WIDTH + extraSize;
	float height = WINDOW_HEIGHT + extraSize;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); //preserving the GUI ortho projection
	glLoadIdentity();
	glOrtho(pos.x - width / 2 - vel, pos.x + width / 2 + vel, 
		pos.y + height / 2 + vel, pos.y - height / 2 - vel, -1, 1);

	for (int i = gameObjCount - 1; i>=0; i--) //rendering backwards cause bullets are at the end, but should be hidden by ships
		gameObjects[i]->Render();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); //and get the GUI back

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
		{
			//tree->Insert(obj);
			grid->Add(obj);
			obj->SetCollisionsHandledFlag(false);
		}
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
		for (auto iter2 = cols.begin(); iter2 != cols.end(); iter2++) //this does per pixel 2 times. introduce a flag to limit checks
		{
			cGameObject *obj2 = (*iter2);
			if (obj1 == obj2 || obj2->IsDead() || obj2->GetCollisionsHandledFlag())
				continue; //don't check if same object/dead/already handled collision
			
			int colLayer = obj2->GetCollisionLayer();
			if ((colLayer & colMask) > 0 && //are the object types supposed to collide with each other
				obj1->GetOwner() != obj2->GetOwner() && //friendlies don't collide
				RECTF::Intersects(obj1->GetRect(), obj2->GetRect()) && //raw AABB intersection
				PerPixelCollision(obj1, obj2)) //final perpixel check
			{
				obj1->CollidedWith(obj2);
				obj2->CollidedWith(obj1);
				break; //can collide only once
			}
		}
		obj1->SetCollisionsHandledFlag(true); //this deals with repeated per pixel collision check (A -> B, then B -> A)
	}
}

bool cGame::PerPixelCollision(cGameObject *g1, cGameObject *g2)
{
	//required data
	char *g1Data = g1->GetData();
	char *g2Data = g2->GetData();
	RECTF g1Rect = g1->GetRect();
	RECTF g2Rect = g2->GetRect();
	glm::mat4x4 g1InvTrans = glm::inverse(g1->GetTransform()); //need to figure out a way to use 1 matrix
	glm::mat4x4 g2InvTrans = glm::inverse(g2->GetTransform()); //multiplication instead of 2
	glm::vec2 g1FullSize = g1->GetSize(); //non-transformed size
	glm::vec2 g2FullSize = g2->GetSize();
	glm::vec2 g1HalfSize = glm::vec2((int)g1FullSize.x / 2, (int)g1FullSize.y / 2);
	glm::vec2 g2HalfSize = glm::vec2((int)g2FullSize.x / 2, (int)g2FullSize.y / 2);

	//rect of intersection
	int bottom = min(g1Rect.bottom, g2Rect.bottom);
	int top = max(g1Rect.top, g2Rect.top);
	int left = max(g1Rect.left, g2Rect.left);
	int right = min(g1Rect.right, g2Rect.right);

	for (int y = top; y < bottom; y++) //hello inverted Y axis
	{
		for (int x = left; x < right; x++)
		{
			//Going separate it in 2 parts, part of optimization - if one of them is transparent, why
			//check the other?
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
			if ((byte)g1Data[i1] == 0) //transparent == no collision
				continue;

			//now, repeating it for second object
			glm::vec4 g2Point = g2InvTrans * point;
			glm::vec2 g2ColorP = glm::vec2((int)g2Point.x, (int)g2Point.y) + g2HalfSize;
			if (!(g2ColorP.x >= 0 && g2ColorP.x < g2FullSize.x && g2ColorP.y >= 0 && g2ColorP.y < g2FullSize.y))
				continue;
			int i2 = (int)(g2ColorP.y * g2FullSize.x + g2ColorP.x) * 4 + 3;
			if ((byte)g2Data[i2] == 0)
				continue;

			return true; //both pixels opaque => colliding
		}
	}
	return false;
}

//method for sending a new wave
void cGame::StartLevel(int level)
{
	paused = false;
	currentLevel = level;
	if (level == 0) //spawn the player for the start of the game
	{
		player = new cPlayer(ShipType::Scout);
		gameObjects.push_back(player);
		gameObjCount++;
	}

	int count = level + 1;
	glm::vec2 playerPos = player->GetPosition();
	for (int i = 0; i < count; i++)
	{
		int rnd = rand() % 100;
		ShipType t;
		if (rnd < 10 + (-12 + level) * 2) //random ships based on level
			t = ShipType::Cruiser;
		else if (rnd < 15 + (-8 + level) * 3)
			t = ShipType::Corvette;
		else if (rnd < 20 + (-4 + level) * 5)
			t = ShipType::Fighter;
		else
			t = ShipType::Scout;
			
		cShip *ship = new cShip(t);
		glm::vec2 offset = glm::vec2(rand() % 600 - 300, rand() % 600 - 300); //circle of r=300
		offset.x += glm::sign(offset.x) * 150; //a circle area with removed circle of r=150
		offset.y += glm::sign(offset.y) * 150; //meaning, they spawn from r=150 to r=450
		ship->SetPosition(playerPos + offset);
		gameObjects.push_back(ship);
		ships.push_back(ship);
	}
	gameObjCount += count;
}

//get all the required textures
void cGame::LoadTextures()
{
	textures.insert(make_pair("scout", new cTexture("Textures\\Ships\\scout.png")));
	textures.insert(make_pair("fighter", new cTexture("Textures\\Ships\\fighter.png")));
	textures.insert(make_pair("corvette", new cTexture("Textures\\Ships\\corvette.png")));
	textures.insert(make_pair("cruiser", new cTexture("Textures\\Ships\\cruiser.png")));
	textures.insert(make_pair("missile", new cTexture("Textures\\Weapons\\missile.png")));
	textures.insert(make_pair("bullet", new cTexture("Textures\\Weapons\\bullet.png")));
	textures.insert(make_pair("tutorial", new cTexture("Textures\\tutorial.png")));
	int randomBg = rand() % 6;
	string bg = "Textures\\Backgrounds\\space" + to_string(randomBg) + ".png";
	textures.insert(make_pair("space", new cTexture(bg.c_str())));
}

//get all(2) required sounds
void cGame::LoadSounds()
{
	soundMgr->Add("bullet", "Audio\\Bullet.wav");
	soundMgr->Add("theme", "Audio\\Theme.wav");
}

//clean up
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

//on resize callback
void cGame::OnResize(int width, int height)
{
	windowSize = glm::vec2(width, height);
	gui->UpdateSize(windowSize);
	background->UpdateSize(width, height);
}

//targetting for missiles
cShip* cGame::GetNearestShip(glm::vec2 pos)
{
	float min = 100000000.f;
	cShip *ret = NULL;
	for (cShip *ship : ships)
	{
		glm::vec2 delta = ship->GetPosition() - pos;
		float dist = glm::length2(delta);
		if (dist < min)
		{
			min = dist;
			ret = ship;
		}
	}
	return ret;
}