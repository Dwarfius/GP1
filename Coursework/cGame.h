#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include "cTexture.h"
#include "cGUI.h"
#include "cPlayer.h"
#include "cBackground.h"
#include "cQuadtree.h"
#include "cGrid.h"
#include "cSoundMgr.h"

class cGame
{
private:
	cGame();
	static cGame *singleton;

	cGUI *gui;
	cBackground *background; 
	cPlayer *player = NULL;
	cQuadtree *tree;
	cGrid *grid;
	cSoundMgr *soundMgr;

	bool paused = false;
	vector<cGameObject*> gameObjects;
	vector<cShip*> ships;
	int gameObjCount = 0;
	vector<cGameObject*> objctsToDelete;
	map<string, cTexture*> textures;
	int score = 0;
	int currentLevel = 0;

	bool PerPixelCollision(cGameObject* g1, cGameObject* g2);
	void LoadTextures();
	void LoadSounds();

public:
	~cGame();
	static cGame* Get() { if (!singleton) singleton = new cGame(); return singleton; }

	int GetScore() { return score; }
	void AddScore(int delta) { score += delta; }

	void Update(float delta);
	void CollisionUpdate();
	void Render();

	void AddGameObject(cGameObject *obj) { gameObjects.push_back(obj); gameObjCount++; }
	void StartLevel(int level);
	void StartNextLevel() { StartLevel(++currentLevel); }
	cPlayer* GetPlayer() { return player; };
	void SetPaused(bool state) { paused = state; }
	void Clear();
	int GetGameObjetsCount() { return gameObjCount; }

	void OnResize(int width, int height);
	cShip* GetShipUnderPoint(glm::vec2 pos);

	cTexture* GetTexture(string name) { return textures[name]; }

	void PlaySnd(string name) { soundMgr->GetSnd(name)->Play(false); }
};