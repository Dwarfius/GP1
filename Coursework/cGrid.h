#pragma once

#include "cGameObject.h"

#define NODE_SIZE 50

class cGrid
{
private:
	int width, height;
	int gridW, gridH;
	int count;
	glm::vec2 center;
	vector<cGameObject*> **grid;
	RECTF bounds;

public:
	cGrid(int pWidth, int pHeight);
	~cGrid();

	void SetPos(glm::vec2 pos);
	void Clear();
	void Add(cGameObject *obj);
	void Get(vector<cGameObject*> &ret, RECTF r);
};

