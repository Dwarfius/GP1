#include "cGrid.h"

#pragma warning (disable : 4244)

cGrid::cGrid(int pWidth, int pHeight)
{
	width = pWidth;
	height = pHeight;
	gridW = width / NODE_SIZE;
	gridH = height / NODE_SIZE;
	count = gridW * gridH;
	grid = (vector<cGameObject*>**)malloc(sizeof(vector<cGameObject*>**) * count);
	for (int i = 0; i < count; i++)
		grid[i] = new vector<cGameObject*>();
	bounds = { center.x - width / 2, center.y - height / 2, center.x + width / 2, center.y + height / 2};
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
	if (RECTF::Intersects(bounds, r))
	{
		//transforming to grid nodes
		int top = glm::max((r.top - bounds.top) / NODE_SIZE, 0.f);
		int bottom = glm::min((r.bottom - bounds.top) / NODE_SIZE, (float)gridH - 1);
		int left = glm::max((r.left - bounds.left) / NODE_SIZE, 0.f);
		int right = glm::min((r.right - bounds.left) / NODE_SIZE, (float)gridW - 1);
		for (int y = top; y <= bottom; y++)
			for (int x = left; x <= right; x++)
				grid[y * gridW + x]->push_back(obj);
	}
}

void cGrid::Get(vector<cGameObject*> &ret, RECTF r)
{
	if (!RECTF::Intersects(bounds, r))
		return;

	int top = glm::max((r.top - bounds.top) / NODE_SIZE, 0.f);
	int bottom = glm::min((r.bottom - bounds.top) / NODE_SIZE, (float)gridH - 1);
	int left = glm::max((r.left - bounds.left) / NODE_SIZE, 0.f);
	int right = glm::min((r.right - bounds.left) / NODE_SIZE, (float)gridW - 1);
	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
			ret.insert(ret.end(), grid[y * gridW + x]->begin(), grid[y * gridW + x]->end());
}