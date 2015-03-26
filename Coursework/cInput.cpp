#include "cInput.h"
#include "GameConstants.h"

bool cInput::keys[2][256];
bool cInput::buttons[2][2];
glm::vec2 cInput::mPos;
float cInput::lX, cInput::lY, cInput::rX, cInput::rY;
float cInput::lT, cInput::rT;
bool cInput::controller[10];
float cInput::resetPollingTimer = 0;
int cInput::msgInd;

void cInput::Update(float delta)
{
	memcpy(keys[1], keys[0], sizeof(keys[0]));
	memcpy(buttons[1], buttons[0], sizeof(buttons[0]));

	if (resetPollingTimer -= delta <= 0)
	{
		DWORD res;
		XINPUT_STATE state;
		memset(&state, 0, sizeof(state));
		res = XInputGetState(0, &state);
		if (res == ERROR_SUCCESS)
		{
			msgInd = state.dwPacketNumber;
			//https://msdn.microsoft.com/en-us/library/windows/desktop/ee417001%28v=vs.85%29.aspx

		}
		else
		{
			resetPollingTimer = 2;
		}
	}
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