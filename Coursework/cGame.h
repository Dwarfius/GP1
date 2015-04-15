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
	glm::vec2 windowSize;

	bool PerPixelCollision(cGameObject* g1, cGameObject* g2);
	void LoadTextures();
	void LoadSounds();

public:
	~cGame();
	static cGame* Get() { if (!singleton) singleton = new cGame(); return singleton; }

	//current score
	int GetScore() { return score; }
	void AddScore(int delta) { score += delta; }

	//globals
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

	glm::vec2 GetWindowSize() { return windowSize; }
	void OnResize(int width, int height);

	void PlaySnd(string name, bool loop) { soundMgr->GetSnd(name)->Play(loop); }
	
	//utility
	cSound *GetSnd(string name) { return soundMgr->GetSnd(name); }
	cTexture* GetTexture(string name) { return textures[name]; }
	cShip* GetNearestShip(glm::vec2 pos);
};