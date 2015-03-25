#include "cInput.h"
#include "GameConstants.h"

bool cInput::keys[2][256];
bool cInput::buttons[2][2];
glm::vec2 cInput::mPos;

void cInput::Update()
{
	memcpy(keys[1], keys[0], sizeof(keys[0]));
	memcpy(buttons[1], buttons[0], sizeof(buttons[0]));
}

void cInput::Reset()
{
	memset(keys, 0, sizeof(keys));
	memset(buttons, 0, sizeof(buttons));
}

bool cInput::GetAnyKey()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetKey(i))
			return true;
	}
	return false;
}

bool cInput::GetAnyKeyDown()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetKeyDown(i))
			return true;
	}
	return false;
}