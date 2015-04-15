#include "cGrid.h"

#pragma warning (disable : 4244)

cGrid::cGrid(int pWidth, int pHeight) //creates a fixed size grid with fixed amount of nodes
{
	width = pWidth;
	height = pHeight;
	gridW = width / NODE_SIZE;
	gridH = height / NODE_SIZE;
	count = gridW * gridH;
	grid = (vector<cGameObject*>**)malloc(sizeof(vector<cGameObject*>**) * count); //each node can contain a set of objects
	for (int i = 0; i < count; i++)
		grid[i] = new vector<cGameObject*>();
	bounds = { center.x - width / 2, center.y - height / 2, center.x + width / 2, center.y + height / 2 }; //calculating the bounds for ignoring out of bounds objects
}

cGrid::~cGrid()
{
	for (int i = 0; i < count; i++)
		delete grid[i];
	free(grid);
}

void cGrid::SetPos(glm::vec2 pos)
{
	center = pos;
	bounds = { center.x - width / 2, center.y - height / 2, center.x + width / 2, center.y + height / 2 };
}

void cGrid::Clear()
{
	for (int i = 0; i < count; i++)
		grid[i]->clear();
}

void cGrid::Add(cGameObject *obj)
{
	RECTF r = obj->GetRect();
	if (RECTF::Intersects(bounds, r)) //only add the in bounds objects - can ignore other too far of screen
	{
		//transforming to grid nodes
		int top = glm::max((r.top - bounds.top) / NODE_SIZE, 0.f);
		int bottom = glm::min((r.bottom - bounds.top) / NODE_SIZE, (float)gridH - 1);
		int left = glm::max((r.left - bounds.left) / NODE_SIZE, 0.f);
		int right = glm::min((r.right - bounds.left) / NODE_SIZE, (float)gridW - 1);
		for (int y = top; y <= bottom; y++)
			for (int x = left; x <= right; x++)
				grid[y * gridW + x]->push_back(obj); //adding the object in to every node it partially covers
	}
}

void cGrid::Get(vector<cGameObject*> &ret, RECTF r)
{
	if (!RECTF::Intersects(bounds, r)) //if out of bounds  - ignore
		return;

	//transforming the rect in to node coordinates
	int top = glm::max((r.top - bounds.top) / NODE_SIZE, 0.f);
	int bottom = glm::min((r.bottom - bounds.top) / NODE_SIZE, (float)gridH - 1);
	int left = glm::max((r.left - bounds.left) / NODE_SIZE, 0.f);
	int right = glm::min((r.right - bounds.left) / NODE_SIZE, (float)gridW - 1);
	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
			ret.insert(ret.end(), grid[y * gridW + x]->begin(), grid[y * gridW + x]->end()); //collecting every object in the rect
}