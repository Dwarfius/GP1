#include "cQuadtree.h"

#pragma warning(disable : 4018)

//quadtree class, ended up not using it
cQuadtree::cQuadtree(short pDepth, RECTF pRect)
{
	depth = pDepth;
	rect = pRect;
	objects.reserve(cMaxObjects);
	nodes = (cQuadtree**)malloc(sizeof(cQuadtree*) * 4);
	for (int i = 0; i < 4; i++)
		nodes[i] = NULL;
}

cQuadtree::~cQuadtree()
{
	free(nodes);
}

//split the quad
void cQuadtree::Split()
{
	float halfW = (rect.right - rect.left) / 2;
	float halfH = (rect.bottom - rect.top) / 2;
	float x = rect.left;
	float y = rect.top;

	nodes[0] = new cQuadtree(depth + 1, { x + halfW, y, x + halfW * 2, y + halfH });
	nodes[1] = new cQuadtree(depth + 1, { x, y, x + halfW, y + halfH });
	nodes[2] = new cQuadtree(depth + 1, { x, y + halfH, x + halfW, y + halfH * 2});
	nodes[3] = new cQuadtree(depth + 1, { x + halfW, y + halfH, x + halfW * 2, y + halfH * 2});
}

//helper function which determines the quad for the rect
int cQuadtree::GetIndex(RECTF pRect)
{
	float centerX = rect.left + (rect.right - rect.left) / 2;
	float centerY = rect.top + (rect.bottom - rect.top) / 2;

	bool topQuads = pRect.bottom < centerY;
	bool botQuads = pRect.top > centerY;
	if (pRect.left > centerX)
	{
		if (topQuads)
			return 0;
		else if (botQuads)
			return 3;
	}
	else if (pRect.right < centerX)
	{
		if (topQuads)
			return 1;
		else
			return 2;
	}
	return -1;
}

//cleaning the tree
void cQuadtree::Clear()
{
	objects.clear();
	for (int i = 0; i < 4; i++)
	{
		if (nodes[i])
		{
			nodes[i]->Clear();
			nodes[i] = NULL;
		}
	}
}

//adding the object
void cQuadtree::Insert(cGameObject *obj)
{
	if (nodes[0])
	{
		int index = GetIndex(obj->GetRect());
		if (index != -1)
		{
			nodes[index]->Insert(obj);
			return;
		}
	}

	objects.push_back(obj);
	if (objects.size() > cMaxObjects && depth < cMaxDepth) //if we pushed too many - split
	{
		if (!nodes[0])
			Split();

		int i = 0;
		while (i < objects.size())
		{
			int index = GetIndex(objects[i]->GetRect()); //if fits a child quad - push it there
			if (index != -1)
			{
				nodes[index]->Insert(objects[i]);
				objects.erase(objects.begin() + i);
			}
			else //otherwise let it stay in parent quad
				i++;
		}
	}
}

void cQuadtree::Get(vector<cGameObject*> &ret, RECTF pRect)
{
	int index = GetIndex(pRect);
	if (index != -1 && nodes[0])
		nodes[index]->Get(ret, pRect); //recursively gather up the objects for quads that fit the pRect

	ret.insert(ret.begin(), objects.begin(), objects.end());
}