#pragma once

#include "cGameObject.h"

//http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
class cQuadtree
{
private:
	const int cMaxObjects = 10;
	const int cMaxDepth = 5;

	short depth;
	vector<cGameObject*> objects;
	RECTF rect;
	cQuadtree **nodes;

	void Split();
	int GetIndex(RECTF pRect);

public:
	cQuadtree(short pDepth, RECTF pRect);
	~cQuadtree();
	
	void Clear();
	void Insert(cGameObject *obj);
	void Get(vector<cGameObject*> &ret, RECTF pRect);
};

